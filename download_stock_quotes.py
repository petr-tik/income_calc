#! /usr/bin/env python
"""
Script to download prices of different companies' stock
used to calculate the value of shares in an offer.
Ran during make by downloading a csv of Stock Prices from Yahoo.Finance
for most popular tech companies (list below).
Ran from makefile, so it prints to stdout, rather than logs.

If the OPTION_FLAGS determines the format of the csv, which is later read by a function in C.

TODO: add tests to assert that all relevant information is always present. If you change the flags - need to rerun the tests.

Running:

the script is run without args during installation and at any other time,
when you a new file needs to be created

python download_stock_quotes.py

When a stock_quote is missing from the file, run the script
with sys.argv[2] = stock_quote

python download_stock_quotes.py $STOCK_QUOTE

"""

import requests
from string import join
import shutil
import sys
import os.path

TIMEOUT_VAL = 10

DEFAULT_STOCK_QUOTES = ["AAPL", "MSFT", "INTC", "IBM", "CSCO", "ORCL",
                        "GOOG", "FB", "NVDA", "YHOO", "LNKD", "TWTR", "YNDX"]
# TODO - include MAIL.RU stock index

OUTPUT_FILENAME = ".stocks.csv"


def generate_stock_quote_str(new_stock_quote):
    """ Given new_stock_quotes (None, one string or list of string)
    returns a string of stock_quotes joined by + (Yahoo.Finance separator
    """
    if new_stock_quote is None:
        stock_quotes_str = join(DEFAULT_STOCK_QUOTES, "+")
    elif isinstance(new_stock_quote, list):
        new_stock_quote = map(upper,
                              filter(lambda x: x not in DEFAULT_STOCK_QUOTES))

        stock_quotes_str = join(new_stock_quote, "+")
    return stock_quotes_str


def make_url_for_stocks(new_stock_quote=None):
    """ Fills a yahoo finance URL with option flags and stock quotes.
    Takes no args, 1 string arg or a list of strings
    Default case is used at compile/install time, makes a url of default stocks
    1 string arg - when one stock is missing and needs to be

    """
    stock_quotes_str = generate_stock_quote_str(new_stock_quote)
    option_flags = "nsm3"
    # n - name, s - symbol, m3 - 50 days' moving average
    # look up flags from here
    # http://www.jarloo.com/yahoo_finance/
    url = "https://download.finance.yahoo.com/d/quotes.csv\
?s={}&f={}".format(stock_quotes_str, option_flags)
    return url


def download_stock_prices(
        url,
        open_method,
        output_fname=OUTPUT_FILENAME,
        timeout=TIMEOUT_VAL):
    """ Takes a URL with filled-in option flags and stock_quotes
    and saves it to a csv file.
    Open_methods:
        write - if no file exists
        append - if file exists and stock_quote is new.
                 adds a row with the info of that stock quote
    """
    print "Downloading stock data from {}".format(url)
    fopen_method = open_method + "b"
    try:
        req = requests.get(url, timeout=timeout, stream=True)
        # HTTP errors are not raised by default, this statement does that
        req.raise_for_status()
        if req.status_code == 200:
            with open(output_fname, fopen_method) as f:
                req.raw.decode_content = True
                shutil.copyfileobj(req.raw, f)
        return "Success! Data saved under {}".format(output_fname)
    except requests.HTTPError as e:
        return "Error! Checking connection failed, status code {}.".format(
            e.response.status_code)
    except requests.ConnectionError:
        return "Error! No internet connection available."


if __name__ == "__main__":
    if len(sys.argv) == 1:
        open_method = "w"
        url = make_url_for_stocks()
    elif len(sys.argv) == 2 and os.path.exists(OUTPUT_FILENAME):
        open_method = "a"
        url = make_url_for_stocks(sys.argv[1])
    else:
        exit(1)
    response = download_stock_prices(url, open_method)
    print response
