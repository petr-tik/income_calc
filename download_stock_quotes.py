#! /usr/bin/env python
"""
Script to download prices of different companies' stock 
used to calculate the value of shares in an offer. 
Ran during make by downloading a csv of Stock Prices from Yahoo.Finance 
for most popular tech companies (list below). 
Ran from makefile, so it prints to stdout, rather than logs. 

If the OPTION_FLAGS determines the format of the csv, which is later read by a function in C. 

TODO: add tests to assert that all relevant information is always present. If you change the flags - need to rerun the tests.
"""

import requests
from string import join
import shutil
import sys
import os.path

TIMEOUT_VAL = 10

STOCK_QUOTES = ["AAPL", "MSFT", "INTC", "IBM", "CSCO", "ORCL", "GOOG", 
                "FB", "NVDA", "YHOO", "LNKD", "TWTR", "YNDX"]
# TODO - include MAIL.RU stock index

# n - name, s - symbol, m3 - 50 days' moving average
# look up flags from here
# http://www.jarloo.com/yahoo_finance/
OUTPUT_FILENAME = ".stocks.csv"

def make_url_for_stocks(new_stock_quote=None):
    """ Fills a yahoo finance URL with option flags and stock quotes """
    STOCK_QUOTES_STR = join(STOCK_QUOTES, "+")
    if new_stock_quote != None and new_stock_quote not in STOCK_QUOTES:
        STOCK_QUOTES_STR = new_stock_quote
    OPTION_FLAGS = "nsm3"
    URL = "http://download.finance.yahoo.com/d/quotes.csv\
?s={}&f={}".format(STOCK_QUOTES_STR, OPTION_FLAGS)
    return URL

def download_stock_prices(url, open_method, timeout=TIMEOUT_VAL):
    """ Takes a URL with filled-in option flags and stock_quotes 
    and saves it to a csv file. 
    Open_methods:
        write - if no file exists
        append - if file exists and stock_quote is new
    """
    print "Downloading stock data"
    fopen_method = open_method + "b"
    try:
        req = requests.get(url, timeout=timeout, stream=True)
        # HTTP errors are not raised by default, this statement does that
        req.raise_for_status()
        if req.status_code == 200:
            with open(OUTPUT_FILENAME, fopen_method) as f:
                req.raw.decode_content = True
                shutil.copyfileobj(req.raw, f)
        print "Success! Data saved under {}".format(OUTPUT_FILENAME)
        return True
    except requests.HTTPError as e:
        print "Error! Checking connection failed, status code {}.".format(\
                                                    e.response.status_code)
    except requests.ConnectionError:
        print "Error! No internet connection available."
    return False


if __name__ == "__main__":
    if len(sys.argv) == 1:
        open_method = "w"
        URL = make_url_for_stocks()        
    elif len(sys.argv) == 2 and os.path.exists(OUTPUT_FILENAME): 
        open_method = "a"
        URL = make_url_for_stocks(sys.argv[1])
    else:
        exit(1)
    download_stock_prices(URL, open_method)
