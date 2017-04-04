# Running Intu on your devices

This page will walk you through how to do the following tasks:

1. [Running Intu through the Intu Manager](#intu-through-manager)
2. [Running Intu from the SDK](#intu-sdk)
3. [Updating the config.json](#updating-config)

You will run **Intu** differently depending on whether you installed Intu through the **Intu Manager** or are running the **Intu SDK**:

* If you installed **Intu** on your device through the **Intu Manager** downloaded from the Intu Gateway, follow [these instructions](#intu-through-manager).

* If you are running the **Intu SDK** downloaded or cloned from GitHub, follow [these instructions](#intu-sdk).

## <a name="intu-through-manager">Running Intu through the Intu Manager</a>
If you installed **Intu** through the Intu Manager, follow these steps to run Intu (choose your platform):

### Windows or OS X
1. Open the **Intu Manager**
2. Login using your Bluemix credentials
3. Select your organization and group
4. You will see a list of your devices registered to your organization
5. Click the on/off toggle for your laptop
6. A terminal will pop up and Intu will be running

### Raspberry Pi or Nao or Pepper robot
1. ssh onto your device
2. Navigate to the following directory: `self/latest`
3. On Raspberry Pi, run the following command: `./run_self.sh`
4. On Nao or Pepper, run the following command: `./self_instance`
5. If you get an error, run `export LD_LIBRARY_PATH=./` and then redo step 3 or 4.


## <a name="intu-sdk">Running the Intu SDK</a>
If you downloaded the **Intu SDK** and plan on running that instance of Intu, it is first assumed that you have [compiled](../installation/compiling.md) your instance of Intu already. Next follow the instructions based off of which platform you intend to run Intu on:

### Windows
1. Open the Intu SDK project in Visual Studio
2. Build the project
3. In the Solution Explorer, got to **sdk -> bin**
4. If there is a `config.json` file present, delete its current contents
5. If there is no `config.json` file present, create a file and name it `config.json`
6. Add your configuration to the `config.json` file by following [these instructions](#updating-config)
6. Run Intu

### OS X
1. Navigate to your Intu SDK root directory in your terminal
2. Navigate to the `bin/mac` directory
3. If there is a `config.json` file present, delete its current contents
4. If there is no `config.json` file present, create a file and name it `config.json`
5. Add your configuration to the `config.json` file by following [these instructions](#updating-config)
6. Run the following command: `./self_instance`
7. If you get an error, run `export LD_LIBRARY_PATH=./` and then redo step 6.

### Linux
1. Navigate to your Intu SDK root directory
2. Navigate to the `bin/linux` directory
3. If there is a `config.json` file present, delete its current contents
4. If there is no `config.json` file present, create a file and name it `config.json`
5. Add your configuration to the `config.json` file by following [these instructions](#updating-config)
6. Run the following command: `./self_instance`
7. If you get an error, run `export LD_LIBRARY_PATH=./` and then redo step 6.

### Raspberry Pi
1. ssh onto the Raspberry Pi
2. On the Raspberry Pi, navigate to the Intu SDK root directory
3. Navigate to the `bin/raspi` directory
4. If there is a `config.json` file present, delete its current contents
5. If there is no `config.json` file present, create a file and name it `config.json`
6. Add your configuration to the `config.json` file by following [these instructions](#updating-config)
7. Run the following command: `./run_self.sh`
8. If you get an error, run `export LD_LIBRARY_PATH=./` and then redo step 7.

### Nao and Pepper robots
1. ssh onto the robot
2. On the robot, navigate to the Intu SDK root directory
3. Navigate to the `bin/nao` directory
4. If there is a `config.json` file present, delete its current contents
5. If there is no `config.json` file present, create a file and name it `config.json`
6. Add your configuration to the `config.json` file by following [these instructions](#updating-config)
7. Run the following command: `./self_instance`
8. If you get an error, run `export LD_LIBRARY_PATH=./` and then redo step 7.



## <a name="updating-config">Updating your Configuration</a>
### How to update the config.json file:
1. Login to the [Intu Gateway](https://rg-gateway.mybluemix.net)
2. Click on **VIEW CREDENTIALS** on the lefthand sidebar
3. Selct your Organization and Group
4. Click **Get Credentials**
5. Copy the json displayed
6. In your `config.json` file, paste the copied credentials
7. Save your updated `config.json`
