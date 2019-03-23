import subprocess
import yaml
from termcolor import colored


def run(cmd):
    return subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

nStr=""
nTest=0
nFail=0
nOk=0

with open("cmd.yaml", "r") as f:
    tests = yaml.load(f)

for test, data in tests.items():
    ref = run(data["ref"])
    cmd = run("./../build/42sh " + data["cmd"])
    nTest+=1

    errors = []
    if ref.stdout != cmd.stdout:
        errors.append("STDOUT")
    if ref.stderr != cmd.stderr:
        errors.append("STDERR")
    if ref.returncode != cmd.returncode:
        errors.append("RETURN CODE")

    if errors:
        res = ", ".join(errors)
        nStr+=colored("F", 'red')
        nFail+=1
    else:
        "OK"
        nStre+=colored(".", 'green')
        nOk+=1
    print("-" * 14 + "{id}: {desc} [{res}]".format(id=test, desc=data["desc"], res=res) + "-" * 14)
    print("\nCommand tested : \"" + data["ref"] + "\"\n")

    if "STDOUT" in errors:
        print("\t", "stdout returned: " + cmd.stdout.decode())
        print("\t", "stdout expected: " + ref.stdout.decode())

    if "STDERR" in errors:
        print("\t", "stderr returned: " + cmd.stderr.decode())
        print("\t", "stderr expected: " + ref.stderr.decode())

    if "RETURN CODE" in errors:
        print("\t", "return code returned: " + str(cmd.returncode))
        print("\t", "return code expected: " + str(ref.returncode))

    if cmd.stdout.decode()!=ref.stdout.decode or cmd.stderr.decode()!=ref.stderr.decode() or str(cmd.returncode)!=str(ref.retruncode):
        print(colored("\n\tThe test failed\n", 'red'))
    else:
        print(colored("\n\tThe test succeded\n", 'green'))

print("-" * 30 + "Result :" + "-" * 30 + "\n" + nStr + "\n")
print("Test: " + str(nTest))
print(colored("Passed: " + str(nOk), 'green'))
print(colored("Failed: " + str(nFail) + "\n", 'red'))
tot=nOk/nTest
if tot==100:
    print(colored(str(tot) + "% passed" + "\n",'green'))
elif tot<50:
    print(colored(str(tot) + "% passed" + "\n",'red'))
else:
    print(colored(str(tot) + "% passed" + "\n",'yellow'))
