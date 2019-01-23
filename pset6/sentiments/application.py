from flask import Flask, redirect, render_template, request, url_for

import os
import sys
import helpers
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():
    """Search Twitter for a @user and create a pie chart with the results"""
    """of sentiments analysis on their last 100 tweets."""
    
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, 100)
    if tweets == None:
        return redirect(url_for("index"))

    # initialize counts of positive, negative, and neutral tweets
    positive, negative, neutral = 0.0, 0.0, 100.0
    
    # initialize Analyzer
    analyzer = Analyzer(positives, negatives)
    
    # iterate over tweets, classifying them as positive, negative, or neutral,
    # and changing the values of their counting variables
    for i in range(len(tweets)):
        tweet = tweets[i]
        score = analyzer.analyze(tweet)
        if score > 0.0:
            positive += 1.0
            neutral -= 1.0
        elif score < 0.0:
            negative += 1.0
            neutral -= 1.0

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
