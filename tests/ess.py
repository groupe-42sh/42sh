import subprocess
from termcolor import colored

def test_echo_backslash_n():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = (b"echo hello world")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    if out == mout:
        print("error\n" + colored(command.decode(), 'red'))

def test_echo_substitution_back_quote():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("echo a`echo -n lol`b")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

def test_echo_substitution_dollar():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("echo a$(echo -n lol)b")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

def test_ls_bad_option_w():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("ls -w")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(err == merr)

def test_ls():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("ls")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

def test_ls_option_a():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("ls -a")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

def test_assign_variable():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("n=5")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

def test_operation():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("n=6+8")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

test_echo_backslash_n()
