PHP_ARG_ENABLE(nii, whether to enable nii framework, [ --enable-nii   Enable nii framework])

if test "$PHP_NII" = "yes"; then
  AC_DEFINE(HAVE_NII, 1, [Whether you have Nii Framework])

  PHP_NEW_EXTENSION(nii,
    nii.c \
	func.c \
	nii_func.c \
	\
	system/nii.c \
	system/base_nii.c \
	system/base/object.c \
	system/base/component.c \
	system/base/behavior.c \
	\
	dnl exception class
	\
	system/base/exception.c \
	system/base/unknownmethodexception.c \
	system/base/unknownclassexception.c \
	system/base/unknownpropertyexception.c \
	system/base/userexception.c \
	system/base/invalidcallexception.c \
	system/base/invalidconfigexception.c \
	system/base/notsupportedexception.c \
	system/base/invalidparamexception.c \
	system/base/invalidrouteexception.c \
	\
	system/di/container.c \
	system/di/instance.c \
	system/di/servicelocator.c \
	,
  $ext_shared)
fi
