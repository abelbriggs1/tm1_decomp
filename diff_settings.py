def apply(config, args):
    config["arch"] = "mipsel"
    config["baseimg"] = "disks/SIPS_600.07"
    config["myimg"] = "build/TM1.EXE"
    config["mapfile"] = "TM1.map"
    config["source_directories"] = ["."]
    config["objdump_executable"] = "mipsel-linux-gnu-objdump"
