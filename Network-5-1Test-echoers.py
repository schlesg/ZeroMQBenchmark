import subprocess
import os

# print(os.getcwd())
# os.chdir("build/")
commandList = []
commandList.append("./ZMQEchoer --SubEP tcp://192.168.1.105:4242 --PubEP tcp://192.168.1.102:4241")


for command in commandList:
    subprocess.Popen(command.split(), cwd=".")


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
