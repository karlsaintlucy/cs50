import os
import re

from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# https://docs.python.org/3.4/library/xml.etree.elementtree.html
import xml.etree.ElementTree as ET

# https://docs.python.org/3/library/urllib.request.html#module-urllib.request
import urllib.request

# need this to be able to make the geo request for articles URL-ready
import urllib.parse

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo."""
    
    if not request.args.get("geo"):
       raise RuntimeError("Need geo")
    
    # I noted after finishing this function that a bunch of this was done in helpers.py
    # using feedparser, but I'm keeping my work anyway :-D
    
    # https://stackoverflow.com/questions/8840303/urllib2-http-error-400-bad-request
    geo = urllib.parse.quote(request.args.get("geo"))
    google_news_request = "https://news.google.com/news/feeds?geo={}&output=rss".format(geo)
    
    # https://stackoverflow.com/questions/21179272/parsing-a-url-xml-with-the-the-elementtree-xml-api
    feed = ET.parse(urllib.request.urlopen(google_news_request))
    rss = feed.getroot(); channel = rss[0]
    
    # initialize the list that will hold the dictionaries of titles and links from the RSS feed
    articles_list = []
    
    # https://docs.python.org/3.4/library/xml.etree.elementtree.html#finding-interesting-elements
    for item in channel.iter('item'):
        link = item.find('link').text
        title = item.find('title').text
        article_info = dict(zip(['link','title'], [link, title]))
        articles_list.append(article_info)
    
    # return only the first five articles
    return jsonify(articles_list[:5])

@app.route("/search")
def search():
    """Search for places that match query."""
    
    if not request.args.get("q"):
       raise RuntimeError("q")

    # get the queries and spit them out into a list that splits on commas
    query = [s.title() for s in request.args.get("q").split(",")]
    
    # add some regular expression stuff here to test the input -- do I need this?
    
    # if just one term is entered (no commas), or there are two and the second item is null
    # (in the case of a city and a comma)
    if len(query) == 1 or (len(query) == 2 and not query[1]):
        query = query[0]
        rows = db.execute("""SELECT place_name, admin_name1, postal_code, latitude, longitude FROM places
            WHERE place_name LIKE :query OR admin_name1 LIKE :query OR postal_code LIKE :query
            LIMIT 10;""",
            query=query)
    
    # if two terms are entered (one comma): implies City, State
    elif len(query) == 2 and query[1]:
        rows = db.execute("""SELECT place_name, admin_name1, postal_code, latitude, longitude FROM places
            WHERE place_name LIKE :city OR admin_name1 LIKE :state OR admin_code1 LIKE :state
            LIMIT 10;""",
            city=query[0], state=query[1])
    
    # if a city, state, and zip code are entered, only search by zip code
    elif len(query) == 3:
        rows = db.execute("""SELECT place_name, admin_name1, postal_code, latitude, longitude FROM places
            WHERE postal_code LIKE :zipcode
            LIMIT 10;""",
            zipcode=query[2])
    
    return jsonify(rows)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
