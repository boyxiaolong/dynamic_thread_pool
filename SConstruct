import os
def list_dir(dir):
	all_dirs = []
	for root, dirs, files in os.walk('./', True):
		for name in dirs:
			cur_dir = os.path.join(root, name)
			if cur_dir.find(".git") < 0:
				all_dirs.append(cur_dir)
	return all_dirs

cc_flags = {'CCFLAGS' : ['-D_LINUX', '-D_DEBUG', '-g', '-O0',]}
inc_flags = {'CPPPATH':['src/','header/']}
obj = Glob('*.cpp')
all_dirs = list_dir('./')
print(all_dirs)
for dir in all_dirs:
	obj += Glob(dir + '/*.cpp')

#lib1 = File('async-redis-cli/unix-include/lib/libevent.a')
path= ['/bin', '/usr/bin', '/opt/rh/devtoolset-8/root/usr/bin']
env = Environment(ENV={'PATH':path})
env.MergeFlags(inc_flags)
env.MergeFlags(cc_flags)
env.Program('thread_pool', list(obj), LIBS=['pthread'])
