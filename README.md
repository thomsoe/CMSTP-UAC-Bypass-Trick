# CMSTP-UAC-Bypass-Trick
Here is a simple trick to make the UAC Bypass method with CMSTP evade Windows Defender.\
All the PoCs for this UAC bypass are now detected (January 2026), here how to simply bypass UAC with CMSTP on Windows 11 25H2 with Defender enabled.

# Instructions
1. Modify the INF file by replacing with the command you want to execute as an Adminsitrator
2. Drop the INF and the EXE on the victim disk
3. **Only works on cmd.exe** - Launch `.\cmstp-bypass.exe <PATH TO INF>`
4. The command is executed with high integrity level and is not detected by Defender
