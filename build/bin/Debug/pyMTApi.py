# -*- coding: UTF-8 -*-
# encoding: utf-8

import hashlib, os, sys, tempfile, time, signal
from pyMTApi import *

class CMTApi:
    def __init__(self):
        self.api = MTApi()
        self.api.on_exchange_update = self.on_exchange_update
        self.api.on_product_update = self.on_product_update
        self.api.on_commodity_update = self.on_commodity_update

    def Start(self, xml, xmlflag):
        self.api.Start(xml,xmlflag)

    def Stop(self):
        self.api.Stop()

    def on_exchange_update(self, dataset):
        print('on_exchange_update: ', dataset)
        self.dataset = dataset

    def on_product_update(self, dataset, flag):
        print('OnFrontDisconnected: ', dataset)
        self.dataset = dataset

    def on_commodity_update(self, dataset):
        print('OnHeartBeatWarning: ', dataset)
        self.dataset = dataset


if __name__ == '__main__':
    class MyMTApi(CMTApi):
        def on_commodity_update(self, dataset):
            print('MyMTApi on_commodity_update: Login...')
            CMTApi.on_commodity_update(self, dataset)

    mtapi = MyMTApi()
    mtapi.Start('../../../conf/MTSrv.json', 1)

    class test:
        def __init__(self):
            print('test.__init__')

        def __del__(self):
            print('test.__del__')

    a = test()
    del a

    try:
        while 1:
            time.sleep(1)
    except KeyboardInterrupt:
        print("CTRL+C")
        mtapi.Stop()
        del mtapi
        pass