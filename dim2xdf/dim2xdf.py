import sys

def read_dim_file(dim_name):
    with open(dim_name, "rb") as f:
        xdf_bytes = bytearray()
        header_bytes = f.read(0x100)
        if header_bytes[0xab:0xaf] != bytes('DIFC',encoding='utf-8'):
            print("not DIM format (dim_name)")
            return None
        if header_bytes[0] != 0:
            print("unknown format.")
            return None
        for i in range(154):
            n = header_bytes[i+1]
            if (n > 0):
                xdf_bytes += f.read(0x2000)
            else:
                xdf_bytes += bytes([0]*0x2000)
    return xdf_bytes

def write_xdf_image(xdf_name, xdf_bytes):
    if len(xdf_bytes) != 154 * 0x2000:
        print("not XDF image.")
        return
    with open(xdf_name, "wb") as f:
        f.write(xdf_bytes)

def main():
    if len(sys.argv) < 3:
        print("usage: dim2xdf <dim-name> <xdf-name>")
        return 1

    xdf_bytes = read_dim_file(sys.argv[1])
    if xdf_bytes != None:
        write_xdf_image(sys.argv[2],xdf_bytes)

if __name__ == "__main__":
    main()