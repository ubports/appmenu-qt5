TEMPLATE = subdirs

SUBDIRS += src

# Add the enable-by-default flag if you want the profile.d files installed
enable-by-default {
    SUBDIRS += data
}
