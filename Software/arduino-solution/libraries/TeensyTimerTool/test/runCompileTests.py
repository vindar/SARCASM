
libraryName = "TeensyTimerTool"

boards = [
    "teensylc",
    "teensy31",
    "teensy41",
]

examples = [
    {"folder":"examples/01_basic/HelloPeriodic"},
    {"folder":"examples/01_basic/HelloOneShot"},
    {"folder":"examples/01_basic/ArrayOfTimers"},
    {"folder":"examples/01_basic/MoreTimers",             "exclude":["teensy31","teensylc"]},
    {"folder":"examples/01_basic/RTC_Timer",              "exclude":["teensy31","teensylc"]},
    {"folder":"examples/01_basic/UsingChronoDurations1"},
    {"folder":"examples/01_basic/UsingChronoDurations2"},
    {"folder":"examples/02_Advanced/CallbackWithParams"},
    {"folder":"examples/02_Advanced/UsingLambdas"},
    {"folder":"examples/02_Advanced/CapturingLambdas"},
    {"folder":"examples/03_Applications/DoubleExposure",   "exclude":["teensylc","teensy41"]},
    {"folder":"examples/99_Misc/PinInformation",           "exclude":["teensy31"]},
]

dependencies=[
]

#*************************************************************************************************
import subprocess
import tempfile
import os
import distutils.dir_util

print("\n================================ SETTING UP TEST PROJECT ================================")

targetPrjFolder = os.path.join(tempfile.gettempdir(), libraryName)
targetLibFolder = os.path.join(targetPrjFolder,"lib",libraryName)
targetSrcFolder = os.path.join(targetPrjFolder,"src")

if os.path.isdir(targetPrjFolder): # start with a fresh setup
    distutils.dir_util.remove_tree(targetPrjFolder)

distutils.dir_util.mkpath(targetPrjFolder)

initParams = ["pio","project", "init", "-s"]
initParams.append("--project-dir=" + targetPrjFolder)
for board in boards:
    initParams.append("--board=" + board)

opt = "--project-option=lib_deps="
for dep in dependencies:
    opt += (dep + '\n')
initParams.append(opt)

result = subprocess.run(initParams)

print("\n================================== COMPILING EXAMPLES ===================================")

distutils.dir_util.copy_tree("./src",targetLibFolder+"/src")

totalExamples = str(len(examples)*len(boards))
currentExample = 1

for board in boards:
    print('{:57}'.format("SKETCH") + '{:25}'.format("BOARD")+ " STATUS" )
    for example in examples:
        exFolder = example['folder']
        counter =  (f"{currentExample}/{totalExamples}").center(8)
        currentExample += 1
        print("- " + '{:55}'.format(exFolder) + '{:25}'.format(board) + counter, end="\r", flush=True)

        if os.path.isdir(targetSrcFolder):
            distutils.dir_util.remove_tree(targetSrcFolder)
        distutils.dir_util.copy_tree (exFolder,targetSrcFolder)

        print("- " + '{:55}'.format(exFolder) + '{:25}'.format(board), end='', flush=True)

        if not ("exclude" in example and (board in example['exclude'])):
            result = subprocess.run(["pio", "run",  "-e"+board,  "--project-dir=" + targetPrjFolder],capture_output=True)

            if result.returncode == 0:
                print("\033[92m[PASSED]\033[0m"+"       ")
            else:
                print("\033[91m[FAILED]\033[0m"+"       ")
        else:
                print("\033[93m[ N/A  ]\033[0m"+"       ")
    print(' ')