#! /usr/bin/env python

import unittest
from os import remove
import os.path
from string import join
from download_stock_quotes import make_url_for_stocks, download_stock_prices,\
    DEFAULT_STOCK_QUOTES


class TestDownloadScript(unittest.TestCase):

    def setUp(self):
        """ Creates a test_file """
        self.test_fname = ".test_stocks.csv"

    def tearDown(self):
        """ Tidies up .test_stock.csv """
        if os.path.exists(self.test_fname):
            os.remove(self.test_fname)

    def test_make_url_default(self):
        """ No new stock quote means default string is made """
        supposed_url = make_url_for_stocks()
        test_stock_quotes_str = join(DEFAULT_STOCK_QUOTES, "+")
        option_flags = "nsm3"
        url = "https://download.finance.yahoo.com/d/quotes.csv\
?s={}&f={}".format(test_stock_quotes_str, option_flags)
        self.assertEqual(url, supposed_url)


if __name__ == '__main__':
    unittest.main(verbosity=10)
