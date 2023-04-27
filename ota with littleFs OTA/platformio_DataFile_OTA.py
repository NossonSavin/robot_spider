import shutil
import subprocess
import requests
import hashlib

try:
    from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
    from tqdm import tqdm
except ImportError:
    subprocess.run("python -m pip install requests_toolbelt")
    subprocess.run("python -m pip install tqdm")

with open("platformio.ini", "r") as file:
    boardName, boardURL = "error", "error"
    lines = file.readlines()
    for line in lines:
        if line.find('[env:') != -1:
            start = line.find('[env:') + 5
            end = line.find(']')
            boardName = line[start: end]

        if line.find('upload_url =') != -1:
            boardURL = (line.lstrip("upload_url =")).strip()


if boardName == "error" or boardURL == "error":
    print("\033[0;31m"+f" board Info missing boardName={boardName} and boardURL={boardURL} ".center(
        shutil.get_terminal_size().columns, '='))

# build litlefs
subprocess.run(
    f"C:/Users/Home/.platformio/penv/Scripts/platformio.exe run --target buildfs --environment {boardName}")

filesystem_path = f".pio/build/{boardName}/littlefs.bin"

with open(filesystem_path, 'rb') as filesystem:
    md5 = hashlib.md5(filesystem.read()).hexdigest()
    filesystem.seek(0)
    encoder = MultipartEncoder(fields={
        'MD5': md5,
        'filesystem': ('filesystem', filesystem, 'application/octet-stream')}
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

    try:
        response = requests.post(boardURL, data=monitor, headers={
            'Content-Type': monitor.content_type})
        bar.close()

        print("\033[0;32m\n" + (f" Data uploaded {response.text} {response} ").center(
            shutil.get_terminal_size().columns, '=')+"\n")
    except:
        print("\033[0;31m"+" Data upload Failed ".center(
            shutil.get_terminal_size().columns, '=')+"\n")
