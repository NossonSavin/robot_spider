import shutil
import subprocess
import requests
import hashlib
import sys
import os
import configparser

try:
    from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
    from tqdm import tqdm
except ImportError:
    subprocess.run("python -m pip install requests_toolbelt")
    subprocess.run("python -m pip install tqdm")


workDir = sys.argv[1]
uploadType = sys.argv[2]

os.chdir(workDir)

config = configparser.ConfigParser()
config.read('platformio.ini')

boardName = config.sections()[0]
boardURL = config.get(boardName, 'upload_url')
boardName = boardName[4:]


print(os.getcwd())

# build
addToBuildCmd = "run" if uploadType == "firmware" else f"run --target buildfs --environment {boardName}"
subprocess.run(
    f"C:/Users/Home/.platformio/penv/Scripts/platformio.exe {addToBuildCmd}")

fileName = "firmware" if uploadType == "firmware" else "littlefs"
filesystem_path = f".pio/build/{boardName}/{fileName}.bin"

with open(filesystem_path, 'rb') as openFile:
    md5 = hashlib.md5(openFile.read()).hexdigest()
    openFile.seek(0)
    encoder = MultipartEncoder(fields={
        'MD5': md5,
        uploadType: (uploadType, openFile, 'application/octet-stream')}
    )

    bar = tqdm(desc='Upload Progress',
               total=encoder.len,
               dynamic_ncols=True,
               unit='B',
               unit_scale=True,
               unit_divisor=1024
               )

    monitor = MultipartEncoderMonitor(
        encoder, lambda monitor: bar.update(monitor.bytes_read - bar.n))

    # Clearing the Screen
    os.system('cls')

    # print("\033[0;32mBuild Complete")
    print("\033[0;32m\n" + ("Build Complete").center(
        shutil.get_terminal_size().columns, '=')+"\n")
    print("\033[0;36mStarting Upload... \n")
    try:
        response = requests.post(boardURL, data=monitor, headers={
            'Content-Type': monitor.content_type})
        bar.close()

        print("\033[0;32m\n" + (f" Data uploaded {response.text} {response} ").center(
            shutil.get_terminal_size().columns, '=')+"\n")
    except:
        print("\033[0;31m"+" Data upload Failed ".center(
            shutil.get_terminal_size().columns, '=')+"\n")
