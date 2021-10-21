class colors:
    HEADER = '\033[95m'
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    def writeHeader(string):
        return (colors.HEADER + string + colors.ENDC)

    def writeBlue(string):
        return (colors.BLUE + string + colors.ENDC)

    def writeGreen(string):
        return (colors.GREEN + string + colors.ENDC)

    def writeYellow(string):
        return (colors.YELLOW + string + colors.ENDC)

    def writeRed(string):
        return (colors.RED + string + colors.ENDC)

    def writeBold(string):
        return (colors.BOLD + string + colors.ENDC)

    def writeUnderline(string):
        return (colors.UNDERLINE + string + colors.ENDC)
