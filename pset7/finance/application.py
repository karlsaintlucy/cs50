from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

#########
#       #
# INDEX #
#       #
#########

@app.route("/")
@login_required
def index():
    
    stocks = db.execute("SELECT stocks.symbol AS symbol, stocks.name AS name, portfolio.shares AS shares "
        "FROM portfolio "
        "INNER JOIN stocks ON portfolio.stocks_id = stocks.id "
        "INNER JOIN users ON portfolio.users_id = users.id "
        "WHERE users.id = :users_id;", users_id=session["user_id"])

    if not stocks:
        return apology("no stocks yet")
        
    total_value = 0
        
    for i in range(len(stocks)):
        symbol = stocks[i]["symbol"]
        current = lookup(symbol)
        shares = float(stocks[i]["shares"])
        current_price = float(current["price"])
        stocks[i]["current_price"] = current_price
        stocks[i]["current_value"] = current_price * shares
        total_value += stocks[i]["current_value"]
    
    rows = db.execute("SELECT cash FROM users WHERE id = :users_id", users_id=session["user_id"])
    cash = float(rows[0]["cash"])
    
    total_value += cash
    
    info_url = "http://www.marketwatch.com/investing/stock/"
        
    return render_template("index.html", stocks=stocks, cash=cash, total=total_value, info_url=info_url)

#######
#     #
# BUY #
#     #
#######

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST":
        
        if not request.form.get("symbol"):
            return apology("no symbol!", "try again")
            
        stock = lookup(request.form.get("symbol"))
        
        if not stock:
            return apology("invalid symbol :(", "try again")
            
        name = stock["name"]
        price = stock["price"]
        symbol = stock["symbol"]
        
        # log the stock in the stocks table if it's not already there
        stocks_temp = db.execute("SELECT id FROM stocks WHERE symbol = :symbol", symbol=symbol)
        if not stocks_temp:
            db.execute("INSERT INTO stocks(symbol, name) VALUES(:symbol, :name)", symbol=symbol, name=name)
            stocks_temp = db.execute("SELECT id FROM stocks WHERE symbol = :symbol", symbol=symbol)
            
        stocks_id = int(stocks_temp[0]["id"])
        
        shares = float(request.form.get("quantity"))
        
        if not shares:
            return apology("how many shares?", "try again")
        
        # https://stackoverflow.com/questions/3501382/checking-whether-a-variable-is-an-integer-or-not
        if shares <= 0:
            return apology("no negative shares!", "try again")
            
        # shares = float(request.form.get("quantity"))
        
        total = price * shares
            
        available = db.execute("SELECT cash FROM users WHERE id = :users_id", users_id=session["user_id"])
        available = float(available[0]["cash"])
        
        if total > available:
            return apology("not enough cash :(", "try again")
        
        remaining = available - price
        
        # log the transaction
        db.execute("INSERT INTO transactions (users_id, stocks_id, price, shares, type, total) "
            "VALUES(:users_id, :stocks_id, :price, :shares, 'purchase', :total)",
            users_id=session["user_id"], stocks_id=stocks_id, price=price, shares=shares, total=total)
        
        # save the new shares in the user's portfolio
        portfolio_id = db.execute("SELECT id FROM portfolio WHERE stocks_id = :stocks_id AND users_id = :users_id",
            stocks_id=stocks_id, users_id=session["user_id"])
        if not portfolio_id:
            db.execute("INSERT INTO portfolio (users_id, stocks_id, shares)"
                "VALUES(:users_id, :stocks_id, :shares);", users_id=session["user_id"], stocks_id=stocks_id, shares=shares)
        else:
            db.execute("UPDATE portfolio SET shares = shares + :shares WHERE users_id = :users_id AND stocks_id = :stocks_id",
                shares=shares, users_id=session["user_id"], stocks_id=stocks_id)
            
        db.execute("UPDATE users SET cash = cash - :total WHERE id = :users_id", total=total, users_id=session["user_id"])
        
        # redirect user to home page
        return redirect(url_for("index"))
        
    else:
        rows = db.execute("SELECT cash FROM users WHERE id = :users_id", users_id=session["user_id"])
        cash = float(rows[0]["cash"])
        
        return render_template("buy.html", cash=cash)

###########
#         #
# HISTORY #
#         #
###########

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    history = db.execute("SELECT stocks.symbol AS symbol, stocks.name AS name, type, price, shares, total, date_time "
        "FROM transactions "
        "INNER JOIN stocks ON transactions.stocks_id = stocks.id "
        "WHERE users_id = :users_id ORDER BY date_time DESC;", users_id=session["user_id"])
    
    if not history:
        return apology("no history yet!")
    
    for i in range(len(history)):
        history[i]["price"] = float(history[i]["price"])
        history[i]["shares"] = float(history[i]["shares"])
        history[i]["total"] = float(history[i]["total"])
    
    info_url = "http://www.marketwatch.com/investing/stock/"
        
    return render_template("history.html", history=history, info_url=info_url)

#########
#       #
# LOGIN #
#       #
#########

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        username = request.form.get("username")

        # ensure username was submitted
        if not username:
            return apology("username?", "try again")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("password?", "try again")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid!", "try again")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        
        return render_template("login.html")

##########
#        #
# LOGOUT #
#        #
##########

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

#########
#       #
# QUOTE #
#       #
#########

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST":
        
        if not request.form.get("symbol"):
            return apology("symbol?", "try again")
            
        stock = lookup(request.form.get("symbol"))
        info_url = "http://www.marketwatch.com/investing/stock/"
        
        if not stock:
            return apology("invalid symbol :(", "try again")
        
        return render_template("showquote.html",
                name=stock["name"], info_url=info_url, symbol=stock["symbol"], price=stock["price"])
    
    else:
        
        return render_template("quote.html")

############
#          #
# REGISTER #
#          #
############

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id
    session.clear()
    
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        username = request.form.get("username")
        password = request.form.get("password")
        password_confirm = request.form.get("password-confirm")
        
        # ensure username was submitted
        if not username:
            return apology("username?", "try again")

        # ensure password was submitted
        elif not password or not password_confirm:
            return apology("must provide password and confirmation", "try again")
            
        elif password != password_confirm:
            return apology("passwords must match!", "try again")

        # query database for username
        result =  db.execute("SELECT * FROM users WHERE username = :username", username=username)
        
        # if there is no user found matching username
        if not result:
            hashed = pwd_context.hash(password)
            db.execute("INSERT INTO users(username, hash) VALUES(:username, :hashed)", username=username, hashed=hashed)
            
            # automatically sign them in once they've registered
            rows = db.execute("SELECT id FROM users WHERE username = :username", username=username)
            session["user_id"] = int(rows[0]["id"])
            
            # redirect user to home page
            return redirect(url_for("index"))
        
        # if a user is found matching username
        else:
            return apology("user already exists :(", "try again")
            
    else:
        return render_template("register.html")

########
#      #
# SELL #
#      #
########

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # if sell page accessed through sell form
    if request.method == "POST":
        stock_to_sell = request.form.get("stock_to_sell")
        
        # http://www.pythonforbeginners.com/dictionary/python-split
        symbol, price, qty = stock_to_sell.split(",")
        price = float(price)
        qty = float(qty)
        
        if qty == 0:
            return apology("try selling one or more, dummy!")
            
        total_sale = price * qty
        
        # get the stock id from the stocks table
        rows = db.execute("SELECT id FROM stocks WHERE symbol = :symbol", symbol=symbol)
        stocks_id = int(rows[0]["id"])
        
        # log the transaction in the transactions table
        db.execute("INSERT INTO transactions (users_id, price, shares, type, total, stocks_id) "
            "VALUES (:users_id, :price, :qty, 'sale', :total_sale, :stocks_id);",
            users_id=session["user_id"], price=price, qty=qty, total_sale=total_sale, stocks_id=stocks_id)
        
        # update the portfolio table
        db.execute("UPDATE portfolio SET shares = shares - :qty WHERE users_id = :users_id AND stocks_id = :stocks_id;",
            qty=qty, users_id=session["user_id"], stocks_id=stocks_id)
        
        # remove all records from portfolio that have no shares left (but retain them in transactions table)
        db.execute("DELETE FROM portfolio WHERE users_id = :users_id AND shares = 0;", users_id=session["user_id"])
        
        # increase the user's cash by sale amount
        db.execute("UPDATE users SET cash = cash + :total_sale WHERE id = :users_id", total_sale=total_sale, users_id=session["user_id"])
        
        # go back to index
        return redirect(url_for("index"))
    
    # if coming from another page, show all stocks and share amounts in a table
    # with a dropdown and "sell" buttons for each holding using sell.html
    else:
        # the below is copied from index
        stocks = db.execute("SELECT stocks.symbol AS symbol, stocks.name AS name, portfolio.shares AS shares "
            "FROM portfolio "
            "INNER JOIN stocks ON portfolio.stocks_id = stocks.id "
            "INNER JOIN users ON portfolio.users_id = users.id "
            "WHERE users.id = :users_id;", users_id=session["user_id"])
        
        if not stocks:
            return apology("no stocks yet", "buy some!")
        
        total_value = 0
        
        for i in range(len(stocks)):
            symbol = stocks[i]["symbol"]
            current = lookup(symbol)
            shares = int(stocks[i]["shares"])
            current_price = float(current["price"])
            stocks[i]["current_price"] = current_price
            stocks[i]["current_value"] = current_price * shares
            total_value += stocks[i]["current_value"]
    
        rows = db.execute("SELECT cash FROM users WHERE id = :users_id", users_id=session["user_id"])
        cash = float(rows[0]["cash"])
    
        total_value += cash
        
        info_url = "http://www.marketwatch.com/investing/stock/"
        
        return render_template("sell.html", stocks=stocks, cash=cash, total=total_value, info_url = info_url)

###################
#                 #
# CHANGE PASSWORD #
#                 #
###################

@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changepassword():
    """ Change password. """
    
    if request.method == "POST":
        
        # SANITY CHECKS
        
        # if there's no old password typed in
        if not request.form.get("password-old"):
            return apology("need old password")
        
        # verify old password
        rows = db.execute("SELECT hash FROM users WHERE id = :users_id;", users_id=session["user_id"])
        if not pwd_context.verify(request.form.get("password-old"), rows[0]["hash"]):
            return apology("wrong old password")
        
        # if one of the new password fields is not entered
        if not request.form.get("password-new") or not request.form.get("password-confirm"):
            return apology("need new password and confirmation")
        
        # if the new password fields don't match
        if request.form.get("password-new") != request.form.get("password-confirm"):
            return apology("new password and confirmation must match")
        
        # if the new password is the same as the old password
        if request.form.get("password-old") == request.form.get("password-new"):
            return apology("new password must be different")

        # update the hash in the database
        hashed = pwd_context.hash(request.form.get("password-new"))
        db.execute("UPDATE users SET hash = :hashed WHERE id = :users_id;", hashed=hashed, users_id=session["user_id"])
        
        # forget any user_id
        session.clear()
        
        # redirect user to home page
        return redirect(url_for("index"))
        
    else:
        return render_template("changepassword.html")