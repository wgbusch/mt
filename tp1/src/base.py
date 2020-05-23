import numpy as np
import pandas as pd
import math
import plotly.graph_objs as go
from plotly.offline import download_plotlyjs, init_notebook_mode, plot, iplot
import chart_studio.plotly as py
class Instancia():
    def __init__(self,csvfile,contarEmpates=False,empateSuma=True):
        self._equipos=0
        self._partidos=0
        self._cmm = None
        self.mass=None
        self.diferencia=None
        self.ganados = None
        self.enfrentamientos = None
        self.ganadosEmpates = None
        self.totales = None
        self.ratings=None
        f = open(csvfile, "r")
        i=0
        for line in list(f):
            if i == 0: # header
                i+=1
                columns = line.split()
                self._equipos = int(columns[0])
                self._partidos = int(columns[1])
                self._cmm = np.empty([self._equipos,self._equipos])
                self.ganados= np.empty([self._equipos,self._equipos])
                self.enfrentamientos= np.empty([self._equipos,self._equipos])
                self.totales = np.zeros(self._equipos)
#                 self.diferencias = np.zeros(self._equipos)
                self.ratings = np.ones(self._equipos)*100
                for i in range(0,self._equipos):
                    for j in range(0,self._equipos):
                        self.ganados[i,j]=0.0
                        self.ganados[j,i]=0.0
                        self.enfrentamientos[i,j]=0.
                        self.enfrentamientos[j,i]=0.
            else:
                data = line.split();
                print(data)

                equipo1=int(data[1])-1
                goles1=int(data[2])-1
                equipo2=int(data[3])-1
                goles2=int(data[4])-1
#                 solo aplica para colley
                if((contarEmpates and goles1==goles2) or goles1-goles2!=0):
                    self.totales[int(data[1])-1]+=1
                    self.totales[int(data[3])-1]+=1
                    

#                 self.diferencias[int(data[1])-1]+=int(data[2])
#                 self.diferencias[int(data[1])-1]-=int(data[4])
#                 self.diferencias[int(data[3])-1]+=int(data[4])
#                 self.diferencias[int(data[3])-1]-=int(data[2])
                
                r1= self.getEloRating(equipo1,equipo2,goles1,goles2,True)
                r2 =self.getEloRating(equipo2,equipo1,goles2,goles1)
                
                self.ratings[equipo1]=r1
                self.ratings[equipo2]=r2
                if contarEmpates and goles1==goles2:
                    self.enfrentamientos[equipo1,equipo2]+=1
                    self.enfrentamientos[equipo2,equipo1]+=1
                elif goles1-goles2!=0:
                    self.enfrentamientos[equipo1,equipo2]+=1
                    self.enfrentamientos[equipo2,equipo1]+=1
                    
                if goles1>goles2:
                    print('gana '+str(data[1]))
                    self.ganados[equipo1,equipo2] +=1
                elif goles2>goles1:
                    print('gana '+str(data[3]))
                    self.ganados[equipo2,equipo1] +=1
                elif goles1==goles2 and contarEmpates:
                    print('empate' + str(data[1])+'-'+str(data[3]))
                    if(empateSuma):
                        self.ganados[equipo2,equipo1] +=1
                        self.ganados[equipo1,equipo2] +=1
                        
                    
#             construyo la matriz de cmm
        for i in range(0, ins.equipos()):
            for j in range(0, ins.equipos()):
                if i==j:
                    self._cmm[i][j]=  self.totales[i]+ 2
                else:
                    self._cmm[i][j]=  self.enfrentamientos[i,j]*-1         
#         Construyo el vector b                  
        self.b = np.zeros(self.equipos())
        for i in range(0,self.equipos()):
            ganados =0
            for j in range(0,self.equipos()):
                ganados += self.ganados[i][j]
            self.b[i]= 1+(2*ganados-self.totales[i])/2
                          
    def getEloRating(self,equipo1,equipo2,goles1,goles2,esLocal=False,K=60):
        r0_1 = self.ratings[equipo1]
        r0_2 = self.ratings[equipo2]
        
        df1 = int(goles1-goles2)
        G =0
        if df1==0 or df1==1:
            G=1
        elif df1 == 2:
            G = 1.5
        else:
            G=(11+ abs(df1))/8
#         // W is the result of the game (1 for a win, 0.5 for a draw, and 0 for a loss).
        W=0
        if df1==0:
            W=0.5;
        elif df1>0:
            W=1;

        dr=.0;
        if esLocal :
            dr = (r0_1-r0_2+100)*-1;
        else:
            dr = (r0_1-r0_2)*-1;

        W_e = 1 / (pow(10,(dr/400)) + 1);
        return r0_1+ G*(W-W_e);

    def equipos(self):
        return self._equipos
    def partidos(self):
        return self._partidos
    def cmm(self):
        
        return self._cmm
    def wp(self):
        wp = np.zeros(self._equipos)
        for i in range(0,self._equipos):
            wp[i]=sum(self.ganados[i])/self.totales[i]
        
        return wp

def forward_elimination(A, b, n):
    """
    Calculates the forward part of Gaussian elimination.
    """
    for row in range(0, n-1):
        for i in range(row+1, n):
            factor = A[i,row] / A[row,row]
            for j in range(row, n):
                A[i,j] = A[i,j] - factor * A[row,j]

            b[i] = b[i] - factor * b[row]

#         print('A = \n%s and b = %s' % (A,b))
    return A, b

def back_substitution(a, b, n):
    """"
    Does back substitution, returns the Gauss result.
    """
    x = np.zeros((n,1))
    x[n-1] = b[n-1] / a[n-1, n-1]
    for row in range(n-2, -1, -1):
        sums = b[row]
        for j in range(row+1, n):
            sums = sums - a[row,j] * x[j]
        x[row] = sums / a[row,row]
    return x

def gauss(A, b):
    """
    This function performs Gauss elimination without pivoting.
    """
    n = A.shape[0]

    # Check for zero diagonal elements
    if any(np.diag(A)==0):
        raise ZeroDivisionError(('Division by zero will occur; '
                                  'pivoting currently not supported'))
    
    A, b = forward_elimination(A, b, n)
    return back_substitution(A, b, n)