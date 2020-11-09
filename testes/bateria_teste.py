import time

last_tempo = 0

class Comando():
    def __init__(self,comando):
        self.comando = comando
    
    def envia(self, cmd):
        self.cmd = cmd
        print(self.cmd)
        

comando = Comando('cmd')

comando.envia(12)

""" while(True):
    tempo = int(time.time())
    if last_tempo != tempo:
        last_tempo = tempo
        print(tempo)
 """    
