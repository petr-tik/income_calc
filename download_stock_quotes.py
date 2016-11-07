#! /usr/bin/env python
"""
Script to download prices of different companies' stock 
used to calculate the value of shares in an offer. 
Ran during make by downloading a csv of Stock Prices from Yahoo.Finance 
for most popular tech companies (list below). 
Ran from makefile, so it prints to stdout, rather than logs. 

If the OPTION_FLAGS determines the format of the csv, which is later read by a function in C. 

xTODO: add tests to assert that all relevant information is always present. If you change the flags - need to rerun the tests.
"""

import requests
from string import join
import shutil

TIMEOUT_VAL = 10

STOCK_QUOTES = ["AAPL", "MSFT", "INTC", "IBM", "CSCO", "ORCL", "GOOG", 
                "FB", "NVDA", "YHOO", "LNKD", "TWTR", "YNDX"]
# TODO - include MAIL.RU stock index
OPTION_FLAGS = "nm3"
# n - name, m3 - 50 days' moving average
# look up flags from here
# http://www.jarloo.com/yahoo_finance/
URL = "http://download.finance.yahoo.com/d/quotes.csv\
?s={}&f={}".format(join(STOCK_QUOTES, "+"), OPTION_FLAGS)
OUTPUT_FILENAME = "stocks.csv"

def download_stock_prices(url, timeout=TIMEOUT_VAL):
    try:
        req = requests.get(url, timeout=timeout, stream=True)
        # HTTP errors are not raised by default, this statement does that
        req.raise_for_status()
        if req.status_code == 200:
            with open(OUTPUT_FILENAME, 'wb') as f:
                req.raw.decode_content = True
                shutil.copyfileobj(req.raw, f)
        return True
    except requests.HTTPError as e:
        print "Checking connection failed, status code {}.".format(\
                                                    e.response.status_code)
    except requests.ConnectionError:
        print "No internet connection available."
    return False


if __name__ == "__main__":
    download_stock_prices(URL)
        
    
