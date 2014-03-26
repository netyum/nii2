PHP_ARG_ENABLE(nii, whether to enable nii framework, [ --enable-nii   Enable nii framework])

if test "$PHP_NII" = "yes"; then
  AC_DEFINE(HAVE_NII, 1, [Whether you have Nii Framework])

  PHP_NEW_EXTENSION(nii,
    nii.c \
	func.c \
	\
	system/base/object.c \
	system/base/exception.c \
	,
  $ext_shared)
fi
