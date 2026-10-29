# ------------------------------------------------------------
#
# IMOL (Infinite Modules On Line)
#
# @brief: A framework to seperate modules
# @author: LuoQi
#
# ------------------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += core bwaf program
core.subdir = src/core
bwaf.subdir = src/bwaf
program.subdir = src/program

bwaf.depends = core
program.depends = core bwaf


