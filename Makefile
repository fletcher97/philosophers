################################################################################
# Makefile
################################################################################

# Makefile by fletcher97
# Version: 2.3
# Repo: www.github.com/fletcher97/utils

# v2.3: A rule to check if a program can be compiled was added in other to be
# used for git hooks. A folder with hooks can be found in the same repository
# this makefile came from.

# As of version 2.2 this Makefile expects an asan.c file to be present in the
# asan folder inside the SRC_ROOT directory. A copy of the file is provided
# with the Makefile. Also it now uses clang instead of gcc.

# This makefile can be copied to a directory and it will generate the file
# structure and initialize a git repository with the .init rule. Any variables
# and rules for the specifique project can be added in the appropriate section.

# By default this makefile assumes that libft, 42's student made library, a copy
# of which can be obtained by cloning https://github.com/fletcher97/libft.git,
# is being used. It can be removed by simply commenting any reference to it on
# the library section.

################################################################################
# Project Variables
################################################################################

# Name of a single binary. Add as many variables as required by the project
NAME1 := philo
NAME2 := philo_bonus

# The names of all the binaries. Add aditional variables created above separated
# by space.
NAMES := ${NAME1} ${NAME2}

################################################################################
# Configs
################################################################################

# Verbose levels
# 0: Make will be totaly silenced
# 1: Make will print echos and printf
# 2: Make will not be silenced but target commands will not be printed
# 3: Make will print each command
# 4: Make will print all debug info
#
# If no value is specified or an incorrect value is given make will print each
# command like if VERBOSE was set to 3.
VERBOSE := 1

# Version 2.1 and above of this makefile can generate targets to use other
# makefiles as dependencies. This feature will execute the rule of same name in
# an other makefile. This can be usefull in many situation but also a hinderence
# in others. If for example you just want to clean the root directory the clean
# rule will be executed in any other makefile specified. You can deactivate the
# creation of these targets by setting the bellow variable to 0.
CREATE_LIB_TARGETS := 1

################################################################################
# Compiler & Flags
################################################################################

# Compiler
CC := clang

# Compiler flags
CFLAGS := -Wall -Wextra -Werror -Wvla

# Generic debug flags
DFLAGS := -g

# Address sanitizing flags
ASAN := -fsanitize=address -fsanitize-recover=address
ASAN += -fno-omit-frame-pointer -fno-common
ASAN += -fsanitize=pointer-subtract -fsanitize=pointer-compare
# Technicaly UBSan but works with ASan
ASAN += -fsanitize=undefined
# Technicaly LSan but works with ASan
ASAN += -fsanitize=leak
# Thread sanitizing flags
TSAN := -fsanitize=thread
# Memory sanitizing flags
MSAN := -fsanitize=memory -fsanitize-memory-track-origins

################################################################################
# Root Folders
################################################################################

BIN_ROOT := bin/
DEP_ROOT := dep/
INC_ROOT := inc/
LIB_ROOT := lib/
OBJ_ROOT := obj/
SRC_ROOT := src/

################################################################################
# Libraries
################################################################################

# Libft
# LIBFT_ROOT := ${LIB_ROOT}libft/
# LIBFT_INC := ${LIBFT_ROOT}inc/
# LIBFT := ${LIBFT_ROOT}bin/libft.a

# INC_DIRS += ${LIBFT_INC}
# LIBS += -L${LIBFT_ROOT}bin -lft

# Libraries for which to create default targets. All libraries in this list will
# have targets created autimatically. The targets that are created are set in
# DEFAULT_LIB_RULES. The targets will have to format <library root>//<target>
# and it will invoke make as follows:
# `make -C <library root> <rule>`
# DEFAULT_LIBS := ${LIBFT_ROOT}

# Default targets to create for libraries specified in DEFAULT_LIBS. This is a
# small list of common targets in most makefiles.
# DEFAULT_LIB_RULES := all clean re

# All projects with a copy of this makefile v2.1 and up ate garanteed to work
# with these targets. If you wish to not use them just comment the lines you
# don't want.
# DEFAULT_LIB_RULES += fclean clean_all clean_dep
# DEFAULT_LIB_RULES += debug debug_re debug_asan debug_asan_re

# All projects with a copy of this makefile v2.2 and up ate garanteed to work
# with these targets. If you wish to not use them just comment the lines you
# don't want.
# DEFAULT_LIB_RULES += debug_tsan debug_tsan_re debug_msan debug_msan_re

LIBS += -lpthread

################################################################################
# Content Folders
################################################################################

# Lists of ':' separated folders inside SRC_ROOT containing source files. Each
# folder needs to end with a '/'. The path to the folders is relative to
# SRC_ROOTIf SRC_ROOT contains files './' needs to be in the list. Each list is
# separated by a space or by going to a new line and adding onto the var.
# Exemple:
# DIRS := folder1/:folder2/
# DIRS += folder1/:folder3/:folder4/
DIRS := thread/philo/:thread/:common/
DIRS += process/philo/:process/:common/

SRC_DIRS_LIST := $(addprefix ${SRC_ROOT},${DIRS})
SRC_DIRS_LIST := $(foreach dl,${SRC_DIRS_LIST},$(subst :,:${SRC_ROOT},${dl}))

SRC_DIRS = $(call rmdup,$(subst :,${SPACE},${SRC_DIRS_LIST}))
OBJ_DIRS = $(subst ${SRC_ROOT},${OBJ_ROOT},${SRC_DIRS})
DEP_DIRS = $(subst ${SRC_ROOT},${DEP_ROOT},${SRC_DIRS})

# List of folders with header files.Each folder needs to end with a '/'. The
# path to the folders is relative to the root of the makefile. Library includes
# can be specified here.
INC_DIRS += ${INC_ROOT}

################################################################################
# Files
################################################################################

SRCS_LIST = $(foreach dl,${SRC_DIRS_LIST},$(subst ${SPACE},:,$(strip $(foreach\
	dir,$(subst :,${SPACE},${dl}),$(wildcard ${dir}*.c)))))
OBJS_LIST = $(subst ${SRC_ROOT},${OBJ_ROOT},$(subst .c,.o,${SRCS_LIST}))

SRCS = $(foreach dir,${SRC_DIRS},$(wildcard ${dir}*.c))
OBJS = $(subst ${SRC_ROOT},${OBJ_ROOT},${SRCS:.c=.o})
DEPS = $(subst ${SRC_ROOT},${DEP_ROOT},${SRCS:.c=.d})

INCS := ${addprefix -I,${INC_DIRS}}

BINS := ${addprefix ${BIN_ROOT},${NAMES}}

################################################################################
# Conditions
################################################################################

ifeq ($(shell uname),Linux)
	SED := sed -i.tmp --expression
else ifeq ($(shell uname),Darwin)
	SED := sed -i.tmp
endif

ifeq ($(VERBOSE),0)
	MAKEFLAGS += --silent
	BLOCK := &>/dev/null
else ifeq ($(VERBOSE),1)
	MAKEFLAGS += --silent
else ifeq ($(VERBOSE),2)
	AT := @
else ifeq ($(VERBOSE),4)
	MAKEFLAGS += --debug=v
endif

ifeq (${CREATE_LIB_TARGETS},0)
	undefine DEFAULT_LIBS
endif

################################################################################
# VPATHS
################################################################################

vpath %.o $(OBJ_ROOT)
vpath %.h $(INC_ROOT)
vpath %.c $(SRC_DIRS)
vpath %.d $(DEP_DIRS)

################################################################################
# Project Target
################################################################################

all: ${BINS}

.SECONDEXPANSION:
${BIN_ROOT}${NAME1}: ${LIBFT} $$(call get_files,$${@F},$${OBJS_LIST})
	${AT}printf "\033[33m[CREATING ${@F}]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${@D} ${BLOCK}
	${AT}${CC} ${CFLAGS} ${INCS} ${ASAN_FILE}\
		$(call get_files,${@F},${OBJS_LIST}) ${LIBS} -o $@ ${BLOCK}

${BIN_ROOT}${NAME2}: ${LIBFT} $$(call get_files,$${@F},$${OBJS_LIST})
	${AT}printf "\033[33m[CREATING ${@F}]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${@D} ${BLOCK}
	${AT}${CC} ${CFLAGS} ${INCS} ${ASAN_FILE}\
		$(call get_files,${@F},${OBJS_LIST}) ${LIBS} -o $@ ${BLOCK}

${LIBFT}: $$(call get_lib_target,$${DEFAULT_LIBS},all) ;

################################################################################
# Clean Targets
################################################################################

clean: $$(call get_lib_target,$${DEFAULT_LIBS},$$@)
	${AT}printf "\033[38;5;1m[REMOVING OBJECTS]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${OBJ_ROOT} ${BLOCK}
	${AT}find ${OBJ_ROOT} -type f -name "*.o" -delete ${BLOCK}

fclean: $$(call get_lib_target,$${DEFAULT_LIBS},$$@)
	${AT}printf "\033[38;5;1m[REMOVING OBJECTS]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${OBJ_ROOT} ${BLOCK}
	${AT}find ${OBJ_ROOT} -type f -name "*.o" -delete ${BLOCK}
	${AT}printf "\033[38;5;1m[REMOVING BINARIES]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${BIN_ROOT} ${BLOCK}
	${AT}find ${BIN_ROOT} -type f\
		$(addprefix -name ,${NAMES}) -delete ${BLOCK}

clean_dep: $$(call get_lib_target,$${DEFAULT_LIBS},$$@)
	${AT}printf "\033[38;5;1m[REMOVING DEPENDENCIES]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${DEP_ROOT} ${BLOCK}
	${AT}find ${DEP_ROOT} -type f -name "*.d" -delete ${BLOCK}

clean_all: fclean clean_dep

re: fclean all

################################################################################
# Debug Targets
################################################################################

debug: CFLAGS += ${DFLAGS}
debug: $$(call get_lib_target,$${DEFAULT_LIBS},$$@) all

obj/asan/asan.o: src/asan/asan.c
	${AT}mkdir -p ${@D} ${BLOCK}
	${AT}${CC} -o $@ -c $< ${BLOCK}

debug_asan: CFLAGS += ${DFLAGS} ${ASAN}
debug_asan: ASAN_FILE = obj/asan/asan.o
debug_asan: $$(call get_lib_target,$${DEFAULT_LIBS},$$@) obj/asan/asan.o all

debug_tsan: CFLAGS += ${DFLAGS} ${TSAN}
debug_tsan: $$(call get_lib_target,$${DEFAULT_LIBS},$$@) all

debug_msan: CFLAGS += ${DFLAGS} ${MSAN}
debug_msan: $$(call get_lib_target,$${DEFAULT_LIBS},$$@) all

debug_re: fclean debug

debug_asan_re: fclean debug_asan

debug_tsan_re: fclean debug_tsan

debug_msan_re: fclean debug_msan

################################################################################
# Utility Targets
################################################################################

.init:
	${AT}printf "\033[33m[CREATING FOLDER STRUCTURE]\033[0m\n" ${BLOCK}
	${AT}mkdir -p ${BIN_ROOT} ${BLOCK}
	${AT}mkdir -p ${DEP_ROOT} ${BLOCK}
	${AT}mkdir -p ${INC_ROOT} ${BLOCK}
	${AT}mkdir -p ${OBJ_ROOT} ${BLOCK}
	${AT}mkdir -p ${SRC_ROOT} ${BLOCK}
	${AT}printf "\033[33m[INITIALIZING GIT REPOSITORY]\033[0m\n" ${BLOCK}
	${AT}git init ${BLOCK}
	${AT}echo "*.o\n*.d\n.vscode\na.out\n.DS_Store" > .gitignore ${BLOCK}
	${AT}date > $@ ${BLOCK}
	${AT}printf "\033[33m[CREATING FIRST COMMIT]\033[0m\n" ${BLOCK}
	${AT}git add .gitignore ${BLOCK}
	${AT}git add $@ ${BLOCK}
	${AT}git add Makefile ${BLOCK}
	${AT}git commit -m "init" ${BLOCK}

# Meta target to force a target to be executed
.FORCE: ;

# Print a specifique variable
print-%: ; @echo $*=$($*)

# List all the targets in alphabetical order
targets:
	${AT}${MAKE} LC_ALL=C -pRrq -f ${CURRENT_FILE} : 2>/dev/null\
		| awk -v RS= -F: '/^# File/,/^# files hash-table stats/\
			{if ($$1 !~ "^[#]") {print $$1}}\
			{if ($$1 ~ "# makefile") {print $$2}}'\
		| sort

compile-test: ${addprefix compile-test/,${NAMES}}

################################################################################
# .PHONY
################################################################################

# Phony clean targets
.PHONY: clean fclean clean_dep clean_all

# Phony debug targets
.PHONY: debug debug_re debug_asan debug_asan_re debug_tsan debug_tsan_re

# Phony utility targets
.PHONY: targets .FORCE compile-test

# Phony execution targets
.PHONY: re all

################################################################################
# Constantes
################################################################################

NULL =
SPACE = ${NULL} #
CURRENT_FILE = ${MAKEFILE_LIST}

################################################################################
# Functions
################################################################################

# Get the index of a given word in a list
_index = $(if $(findstring $1,$2),$(call _index,$1,\
	$(wordlist 2,$(words $2),$2),x $3),$3)
index = $(words $(call _index,$1,$2))

# Get value at the same index
lookup = $(word $(call index,$1,$2),$3)

# Remove duplicates
rmdup = $(if $1,$(firstword $1) $(call rmdup,$(filter-out $(firstword $1),$1)))

# Get files for a specific binary
get_files = $(subst :,${SPACE},$(call lookup,$1,${NAMES},$2))

# Get default target for libs given a rule
get_lib_target = $(foreach lib,$1,${lib}/$2)

################################################################################
# Target Templates
################################################################################

define make_bin_def
${1}: ${2}
endef

define make_obj_def
${1}: ${2} ${3}
	$${AT}printf "\033[38;5;14m[OBJ]: \033[38;5;47m$$@\033[0m\n" $${BLOCK}
	$${AT}mkdir -p $${@D} $${BLOCK}
	$${AT}$${CC} $${CFLAGS} $${INCS} -c $$< -o $$@ $${BLOCK}
endef

define make_dep_def
${1}: ${2}
	$${AT}printf "\033[38;5;13m[DEP]: \033[38;5;47m$$@\033[0m\n" $${BLOCK}
	$${AT}mkdir -p $${@D} $${BLOCK}
	$${AT}$${CC} -MM $$< $${INCS} -MF $$@ $${BLOCK}
	$${AT}$${SED} 's|:| $$@ :|' $$@ $${SED_END} $${BLOCK}
	$${AT}$${SED} '1 s|^|$${@D}/|' $$@ && rm -f $$@.tmp $${BLOCK}
	$${AT}$${SED} '1 s|^$${DEP_ROOT}|$${OBJ_ROOT}|' $$@\
		&& rm -f $$@.tmp $${BLOCK}
endef

define make_lib_def
${1}/${2}: .FORCE
	make -C ${1} ${2}
endef

define make_compile_test_def
compile-test/${1}: .FORCE
	$${AT}printf "\033[33m[TESTING $${@F}]\033[0m\n" $${BLOCK}
	$${AT}$${CC} $${CFLAGS} -fsyntax-only $${INCS} $${ASAN_FILE}\
		$$(call get_files,$${@F},$${SRCS_LIST}) $${BLOCK}
endef

################################################################################
# Target Generator
################################################################################

ifneq (${BIN_ROOT},./)
$(foreach bin,${BINS},$(eval\
$(call make_bin_def,$(notdir ${bin}),${bin})))
endif

$(foreach src,${SRCS},$(eval\
$(call make_dep_def,$(subst ${SRC_ROOT},${DEP_ROOT},${src:.c=.d}),${src})))

$(foreach src,${SRCS},$(eval\
$(call make_obj_def,$(subst ${SRC_ROOT},${OBJ_ROOT},${src:.c=.o}),\
${src},\
$(subst ${SRC_ROOT},${DEP_ROOT},${src:.c=.d}))))

$(foreach lib,${DEFAULT_LIBS},$(foreach target,${DEFAULT_LIB_RULES},$(eval\
$(call make_lib_def,${lib},${target}))))

$(foreach name,$(NAMES),$(eval\
$(call make_compile_test_def,${name})))

################################################################################
# Includes
################################################################################

-include ${DEPS}
