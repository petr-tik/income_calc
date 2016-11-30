#! /usr/bin/env python

import unittest
import os.path
from download_stock_quotes import make_url_for_stocks, download_stock_prices

class TestDownloadScript(unittest.TestCase):
    def setUp(self):
        self.test_fname = ".test_stocks.csv"


    def test_make_url(self):
        
