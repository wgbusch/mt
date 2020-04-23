#!/usr/bin/env python
# -*- coding: utf-8 -*-
import argparse 

def assertRunningAsRoot():
    from os import getuid
    if getuid() != 0:
        print('Ejecutar con permisos de administrador')
        exit(1)
if __name__ == "__main__":

    #assertRunningAsRoot()
    #Parse command line arguments
    parser = argparse.ArgumentParser(description='This is template.')
    parser.add_argument("address", help="")            
    args = parser.parse_args()                                            
