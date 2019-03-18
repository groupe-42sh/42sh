import subprocess

def test_echo_backslash_n():
    proc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    mproc = subprocess.Popen(["bash"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE)
    command = ("echo hello world")
    out, err = proc.communicate(input=command)
    mout, merr = mproc.communicate(input=command)
    assert(out == mout)

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


