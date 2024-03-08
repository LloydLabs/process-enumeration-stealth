# 🚴 process-enumeration-stealth
## What is this?
This is a simple PoC which allows you to return a list of PIDs currently using NTFS, by querying the `\ntfs\` base device (the Windows filesystem base object). Using this method circumvents the need to use typical APIs, such as `NtQuerySystemInformation` or the higher level `EnumProcesses`. Using this non-typical reconnaissance method could allow operators to evade typical monitoring on endpoints 🎉. Thank you to [Jonas Lyk](https://twitter.com/jonaslyk) for originally finding this trick.

## How does this work?
1. Get a handle to `\ntfs\` with `GENERIC_READ | SYNCHRONIZE`
2. Query the information with the `FileProcessIdsUsingFileInformation` class using the `NtQueryInformationFile` API
3. Walk over a `PFILE_PROCESS_IDS_USING_FILE_INFORMATION` list of process IDs

## Other
If you wish to contact me quicker, feel free to on [Twitter](https://twitter.com/LloydLabs) or [e-mail](mailto:me@syscall.party). 
