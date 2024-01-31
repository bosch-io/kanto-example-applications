
import os
import sys 
import argparse

   
def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--target-os', help='Target os')
    parser.add_argument('--target-arch', help='Target architecture')
    parser.add_argument('--output', help='Where the result binary should be moved')
    parser.add_argument('--input', help='The build folder')
    parser.add_argument('--result', help="The name of the produced binary")
    options = parser.parse_args()

    command = "GOOS={} GOARCH={} go build -o {}"

    os.chdir(options.input)
    os.system(command.format(options.target_os, options.target_arch, options.result))
    os.system("mv {} {}".format(options.result, options.output))


if __name__ == "__main__":
    sys.exit(main())

