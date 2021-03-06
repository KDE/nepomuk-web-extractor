#! /usr/bin/env python

from Cheetah.Template import Template
import os
import os.path as osp
from optparse import OptionParser
import subprocess
from subprocess import Popen,PIPE

output_dir = '__output'

required_files = [ 
'plugin.h.template',
'plugin.cpp.template',
'plugin_config.h.in.template',
'executive.h.template',
'executive.cpp.template',
'executivereply.h.template',
'executivereply.cpp.template',
'desktop.template',
'CMakeLists.txt.template',
'kcm.h.template',
'kcm.cpp.template',
'kcm.desktop.template',
'kcm.ui'
]



def prepare():
	for rf in required_files:
		if not osp.lexists(rf):
			print " Couldn't found required template: %s " % (rf, )
			print " Probably you launch script from incorrect directory"
			exit()

	if osp.lexists(output_dir):
		if  not osp.isdir(output_dir):
			print "I have found %s and it is not a directory. May me you launch me from incorrect directory" % (output_dir,)
			exit()

		try :
			print "Removing directory and recreating it"
			subprocess.call(['rm', '-rf' , "./"+output_dir])
			#os.rmdir(output_dir)
			os.mkdir(output_dir)
		except:
			print "Can not remove output directory %s. Remove it yourself and relaunch script" % ( output_dir, )
			exit()
	else :
	        print "%s doesn't exist. Creating it" % output_dir
		os.mkdir(output_dir)





def check_generate(namespace,git_enabled):
	author = namespace.get('author',None)
	mail = namespace.get('mail',None)
	name = namespace.get('name',None)
	version = namespace.get('version',None)
	internal_version = namespace.get('internal_version', None)

	if name is None or len(name) == 0:
		print "Please specify a name"
		exit()

	if version is None:
		namespace['version'] = 1

	if namespace['kcm'] is None:
		namespace['kcm'] = False

	if namespace['kcm_ui'] is None:
		namespace['kcm_ui'] = False

	if internal_version is None:
		namespace['internal_version'] = 0;	
	else:
		# Check that version is int
		try :
			namespace['internal_version'] = int(internal_version)
		except ValueError:
			print "Internal version of the Plugin must be integet number"
			exit()

	if git_enabled:
		if author is None or len(author) == 0:
			author =  Popen(["git", "config", "--get", "user.name"], stdout=PIPE).communicate()[0].rstrip("\n \t")
			namespace['author'] = author

		if mail is None or len(mail) == 0:
			mail = Popen(["git", "config", "--get", "user.email"], stdout=PIPE).communicate()[0].rstrip("\n \t")
			namespace['mail'] = mail
	else:
		if author is None or len(author) == 0:
			namespace['author'] = ""

		if mail is None or len(mail) == 0:
			namespace['mail'] = ""

	# Get year
	year = Popen(["date", "+%Y"], stdout=PIPE).communicate()[0].rstrip("\n \t")
	namespace["year"] = year



	generate(namespace)

def generate(namespace):
	name = namespace['name']
	lname = name.lower()
	output_files = { 
			"%splugin.h" % (lname,) : 'plugin.h.template',
			"%splugin.cpp" % (lname,) : 'plugin.cpp.template',
			"%splugin_config.h.in" % (lname,) : 'plugin_config.h.in.template',
			"%sexecutive.h" % (lname,) : 'executive.h.template',
			"%sexecutive.cpp" % (lname,) : 'executive.cpp.template',
			"%sexecutivereply.h" % (lname,) : 'executivereply.h.template',
			"%sexecutivereply.cpp" % (lname,) : 'executivereply.cpp.template',
			"CMakeLists.txt"  : 'CMakeLists.txt.template',
			"webexplugin_%s.desktop" % (lname,) : 'desktop.template',
			}

	if namespace['kcm'] :
		output_files["%skcm.h" % (lname,)] =  'kcm.h.template'
		output_files["%skcm.cpp" % (lname,)] =  'kcm.cpp.template'
		output_files["webexplugin_%s_kcm.desktop" % (lname,)] = 'kcm.desktop.template'


	for (of,of_template) in output_files.iteritems():
		tclass =  Template.compile(file=of_template)
		res = tclass(namespaces = namespace)
		fo = open("".join([output_dir,'/',of]),"w")
		fo.write(str(res))
		#print res
		# .ui file should not be parsed with cheetah
		# Instead we simply copy it
	if namespace['kcm'] :
		if namespace['kcm_ui']:
			output_ui_file_name = "".join([output_dir,'/',"kcm.ui"])
			subprocess.call(['cp', 'kcm.ui' , output_ui_file_name])



def guiFinished():
	namespace = {}
	namespace['name'] = window.nameLineEdit.text()
	namespace['mail'] = window.mailLineEdit.text()
	namespace['author'] = window.authorlLineEdit.text()
	gui_enabled = window.gitCheckBox.value()
	namespace['use_simple'] = window.simpleCheckBox.value()
	prepare()
	check_generate(namespace,git_enabled)

def runGui():
	from PyQt4 import QtGui
	from ui_dialog import Ui_ParametersDialog

	app = QtGui.QApplication(sys.argv)
	window = QtGui.QDialog()
	ui = Ui_ParametersDialog()
	ui.setupUi(window)
	window.accepted.connect(check_generate)


	window.show()
	app.exec_()


parser = OptionParser()
parser.version = "0.2"
parser.add_option("-g", "--gui", dest="gui", action='store_true')
parser.add_option("-s", "--use-simple", dest="use_simple", action='store_true', help="If enabled, then generated Reply class will inherit SimpleDataPPReply. This is usually more convinient")
parser.add_option("-r", "--plugin-version", dest="version", help="Initial version of the plugin. This is the version that will be used in .desktop file for this plugin and presented to the user")
parser.add_option("-v", "--version", dest="print_version", action='store_true')
parser.add_option("-a", "--git", dest="git_enabled",action='store_true',
		                  help="use git to get missed values")
parser.add_option("-n", "--name", dest = "name", help="plugin name. It is used for generating filenames and class names")
parser.add_option("-t", "--author", dest = "author", help="Author name")
parser.add_option("-m", "--mail", dest = "mail", help="Author mail")
parser.add_option("-i", "--internal-version", dest="internal_version", help="This is the integer number. It is used as internal version of the DataPP. \
		You must increase this number ONLY when there are serious \
		improvments in quality of the generated the Decisions. \
		"
		)
parser.add_option("-k", "--kcm", dest="generate_kcm", action='store_true', help="Use this selector if you want to generate KCModule for you plugin")
parser.add_option("-u", "--kcm-ui", dest="generate_kcm_ui", action='store_true', 
help="Use this selector if you want that generated KCModule include Ui template.\
		The generated .ui file can be opened in designer and edited.\
		This option is taken into account only\
		if KCModule generation is enabled ( --kcm switch )")

(options, args) = parser.parse_args()

if (options.print_version):
	parser.print_version()
	exit()


if options.gui:
	runGui()
else:
	namespace = {}
	namespace['name'] = options.name
	if  options.name is None or len(options.name) == 0 :
		print "You must specify at least name (--name ) or call a gui ( --gui )"
		exit()

	namespace['mail'] = options.mail
	namespace['author'] = options.author
	gui_enabled = options.git_enabled
	namespace['use_simple'] = options.use_simple
	namespace['version'] = options.version
	namespace['internal_version'] = options.internal_version;
	namespace['kcm_ui'] = options.generate_kcm_ui
	namespace['kcm'] = options.generate_kcm
	prepare()
	check_generate(namespace,options.git_enabled)


# vim:set sw=4 ts=4 noet :
