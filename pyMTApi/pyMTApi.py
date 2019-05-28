# -*- coding: UTF-8 -*-
# encoding: utf-8

import hashlib, os, sys, tempfile, time, signal
from pyMTApi import *

class CMTDataSet:
    def __init__(self,dataset):
        self.dataset = dataset
    
    def __del__(self):
        pass

    def __getattr__(self, attr):
        pass

    def __setattr__(self, attr, value):
        pass

    def __getitem__(self, index):
        pass

    def __setitem__(self, index, value):
        pass

    def __str__(self):
        pass

    def __repr__(self):
        pass

class CMTApi:
    def __init__(self):
        self.api = MTApi()
        self.api.on_exchange_update = self.on_exchange_update
        self.api.on_product_update = self.on_product_update
        self.api.on_commodity_update = self.on_commodity_update

    def Start(self, attr):
        self.attr = attr
        self.api.Start(attr)

    def Stop(self):
        self.api.Stop()

    def on_exchange_update(self, dataset, flag):
        print('on_exchange_update: ', dataset, flag)
        self.dataset = dataset

    def on_product_update(self, dataset, flag):
        print('OnFrontDisconnected: ', dataset, flag)
        self.dataset = dataset

    def on_commodity_update(self, dataset, flag):
        print('OnHeartBeatWarning: ', dataset, flag)
        self.dataset = dataset


if __name__ == '__main__':
    class MyMTApi(CMTApi):
        def on_commodity_update(self, dataset, flag):
            print('MyMTApi on_commodity_update: Login...')
            CMTApi.on_commodity_update(self, dataset, flag)

    mtapi = MTApi()
    mtapi.Start('../../../conf/pyMTSrv.json', 1)

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