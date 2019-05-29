import subprocess
import os

# print(os.getcwd())
# os.chdir("build/")
commandList = []

commandList.append("./ZMQInitiator --PubEP ipc:///tmp/MS1 --SubEP ipc:///tmp/dummy --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100" ) #C1
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/MS1 --SubEP ipc:///tmp/dummy --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100" ) #C2
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/MS1 --SubEP ipc:///tmp/dummy --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100" ) #C3
commandList.append("./ZMQInitiator --PubEP ipc:///tmp/MS1 --SubEP ipc:///tmp/dummy --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100" ) #C4
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/MS1 --PubEP ipc:///tmp/MS2") #MS1
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/MS2 --PubEP ipc:///tmp/C5_1,ipc:///tmp/C5_2,ipc:///tmp/C5_3,ipc:///tmp/C5_4") #MS2
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/C5_1 --PubEP ipc:///tmp/dummy") #GW1
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/C5_2 --PubEP ipc:///tmp/dummy") #GW2
commandList.append("./ZMQEchoer --SubEP ipc:///tmp/C5_3 --PubEP ipc:///tmp/dummy") #GW3

# commandList.append("./ZMQInitiator --SubEP ipc:///tmp/C5_4 --PubEP ipc:///tmp/MS1 --msgLength 5000 ") #C5





# commandList.append("./initiator --Trans shmem.ini --SubTopic dummy --PubTopic MS1 --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100") #C1
# commandList.append("./initiator --Trans shmem.ini --SubTopic dummy --PubTopic MS1 --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100") #C2
# commandList.append("./initiator --Trans shmem.ini --SubTopic dummy --PubTopic MS1 --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100") #C3
# commandList.append("./initiator --Trans shmem.ini --SubTopic dummy --PubTopic MS1 --msgLength 5000 --roundtripCount 0 --pubName Dummy --updateRate 100") #C4
# commandList.append("./echoer --Trans shmem.ini --SubTopic MS1 --PubTopic MS2 ") #MS1
# commandList.append("./echoer --Trans shmem.ini --SubTopic MS2 --PubTopic C5 ") #MS2
# commandList.append("./echoer --Trans shmem.ini --SubTopic C5 --PubTopic dummy ") #GW1
# commandList.append("./echoer --Trans shmem.ini --SubTopic C5 --PubTopic dummy ") #GW2
# commandList.append("./echoer --Trans shmem.ini --SubTopic C5 --PubTopic dummy ") #GW3

# commandList.append("./initiator --SubTopic C5 --Trans shmem.ini --PubTopic MS1 --msgLength 5000") #C5

# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic3 --msgLength 50")
# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic4 --msgLength 50")
# commandList.append("./ZMQInitiator --PubEP ipc:///tmp/pingTopic1 --SubEP ipc:///tmp/pongTopic5 --msgLength 50")


# commandList.append("./initiator --SubTopic C5 --PubTopic MS1 --msgLength 5000 --roundtripCount 1000") #C5

for command in commandList:
    subprocess.Popen(command.split(), cwd="build/")


input("Press Enter to shutdown...\n")
os.system("pkill -f ZMQ")
