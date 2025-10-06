import subprocess

result = subprocess.run(["pio", "test", "-ctest/cfgRunUnitTests.ini"])
