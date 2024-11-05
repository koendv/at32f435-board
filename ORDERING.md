# Ordering

These are my notes how to order the pcb's.

## Export design files

- Open project in [easyeda-pro](https://easyeda.com/).
- Open PCB "PCB1".
- Click menu "Export->Bill Of Materials (BOM)". Creates file "BOM_Board1_PCB1_2024-10-10.xlsx"
- Click menu "Export->PCB Fabrication File (Gerber)". Creates file "Gerber_PCB1_2024-10-10.zip"
- Click menu "Export->Pick and Place File". Creates file  "PickAndPlace_PCB1_2024-10-10.xlsx"
- Click menu "Export->3D Shell File". Creates file "3DShell_PCB1.zip".
- Close easyeda-pro

## Order Box

- Unzip 3D Shell File "3DShell_PCB1.zip". Creates files "3DShell_3DShell_PCB1_T.stl" and "3DShell_3DShell_PCB1_B.stl".
- In browser, open [jlcpcb.com](https://jlcpcb.com/). Choose "JLC3DP - 3D printing". Click "Quote Now".
- In browser, click "Add 3D Files". Choose files "3DShell_3DShell_PCB1_T.stl" and "3DShell_3DShell_PCB1_B.stl".
- Choose 3D Technology: SLA (Resin)
 - Choose Material: JLC Black Resin.
 - Choose Shipping: Global Standard Direct Line.
 - Warning appears: "smaller than 0.8mm wall thickness detected. There might be risks of crack, damage, deformation, loss of details. Are you able to take the risks?". Choose "yes".
 - Order.
 
## Order PCB Assembly
 
 - In browser, open [jlcpcb.com](https://jlcpcb.com/). Click "Instant Quote".
 - Click "Add gerber file". Choose file "Gerber_PCB1_2024-10-10.zip".
 - Choose "Impedance Control": yes. JLC0416H-7628.
 - Choose "PCB Assembly", Assemble top side.
 - Choose "PCBA Type": Economic.
 - Choose "PCBA qty": 2.
 - Choose Shipping: Global Standard Direct Line.
 - Click "Next".
 - PCB Image appears. Click "Next".
 - Click "Add BOM File". Choose file "BOM_Board1_PCB1_2024-10-10.xlsx"
 - Click "Add CPL File". Choose file "PickAndPlace_PCB1_2024-10-10.xlsx"
 - Click "Process BOM and CPL".
 - Error appears. "The below parts won't be assembled due to data missing.
... designators don't exist in the BOM file.". Click "Continue".
- Bill of Materials. If all parts available, click "Next".
- If "parts inventory shortage", pre-order  parts, or substitute parts. Search substitute using looking glass next to part. When all parts confirmed, Click "Next".
- Component placement page. Click "Next".
- Quote and order page.
- Product description "Research\Education\DIY\Entertainment", "Development Board - HS Code 847330"
- Click "Save to cart".
- Order.
 
## Order display
 
 - P169H002 capacitive touch screen from [aliexpress](https://aliexpress.com/wholesale?SearchText=P169H002&sortType=total_tranpro_desc).
 - in passing, order JST SH 1.0mm to Dupont cables, also from aliexpress.
 
