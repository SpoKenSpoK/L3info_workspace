import os

def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

def which(program):

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return None

def programsToUse(listOfPrograms):
    ret = {}
    for program in listOfPrograms:
        path=which(program)
        if(path==None):
            pathInSub='fromP9M4/%(Program)s' % {'Program':program}
            thisPath='./%(Program)s' % {'Program':program}
            if(is_exe(pathInSub)):
                path=pathInSub
            elif(is_exe(thispath)):
                path=thisPath
            else:
                raise ValueError('Program %s not found' % program)
        ret.update({program:path})
    return ret
