import subprocess
import yaml
from termcolor import colored


def run(cmd):
    return subprocess.run(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

nTest=0
nFail=0
nOk=0

with open("cmd.yaml", "r") as f:
    tests = yaml.load(f)

for test, data in tests.items():
    ref = run(data["ref"])
    cmd = run("./../build/42sh " + data["cmd"])
    nTest+=1

    #  Listing des erreurs qui sont arrivés
    errors = []
    if ref.stdout != cmd.stdout:
        errors.append("STDOUT")
    if ref.stderr != cmd.stderr:
        errors.append("STDERR")
    if ref.returncode != cmd.returncode:
        errors.append("RETURN CODE")

    if errors:
        res = ", ".join(errors)
        nFail+=1
    else:
        "OK"
        nOk+=1
    print("-" * 30)
    print("{id}: {desc} [{res}]".format(id=test, desc=data["desc"], res=res))

    # decode() convertit des bytes en str
    if "STDOUT" in errors:
        print("\t", "stdout returned: " + cmd.stdout.decode())
        print("\t", "stdout expected: " + ref.stdout.decode())

    if "STDERR" in errors:
        print("\t", "stderr returned: " + cmd.stderr.decode())
        print("\t", "stderr expected: " + ref.stderr.decode())

    if "RETURN CODE" in errors:
        print("\t", "return code returned: " + str(cmd.returncode))
        print("\t", "return code expected: " + str(ref.returncode))


print("Test: " + str(nTest))
print("Passed: " + str(nOk))
print("Failed: " + str(nFail) + "\n")
tot=nOk/nTest
print(str(tot) + "% passed" + "\n")
