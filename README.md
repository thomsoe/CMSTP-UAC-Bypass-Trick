# CMSTP-UAC-Bypass-Trick
Here is a simple trick to make the UAC Bypass method with CMSTP evade Windows Defender.\
All the original PoCs for the CMSTP UAC bypass are now detected (January 2026), here's how to make it work on Windows 11 25H2 with Defender enabled.\
Original research : https://oddvar.moe/2017/08/15/research-on-cmstp-exe/ \
**All credits goes to this man.**

# Instructions
1. Modify the INF file by replacing with the command you want to execute as an Adminsitrator
2. Drop the INF and the EXE on the victim disk
3. **Only works on cmd.exe** - Launch `.\cmstp-bypass.exe <PATH TO INF>`
4. The command is executed with high integrity level and is not detected by Defender
