load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "cli",
    hdrs = glob(["actions/*.h"]) + ["commandLineInterface.h"],
    visibility = ["//visibility:public"],
    alwayslink = 1
)