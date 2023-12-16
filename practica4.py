from __future__ import print_function
import de2120_barcode_scanner
import time
import sys

print("\nScaneo de un codigo de barras o QR:")
my_scanner = de2120_barcode_scanner.DE2120BarcodeScanner()

opcion = 1
if my_scanner.begin() == False:
    print("\nError, no está conectado correctamente")
    opcion = 2

while opcion != 2:
    print("\nSelecciona una opción:")
    print("\n1.Scanear un código")
    print("\n2.Salir")
    opcion = int(input())
    if opcion == 1:
        print("\nCapturando...")

        scan_buffer = ""
        
        while True:
            scan_buffer = my_scanner.read_barcode()
            if scan_buffer:
                print("\nCodigo encontrado: " + str(scan_buffer))
                scan_buffer = ""
                break
            
            time.sleep(0.02)
    if opcion == 2:
        print("\nSaliendo...")
