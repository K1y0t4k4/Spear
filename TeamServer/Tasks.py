from typing import Dict
from rich.console import Console

Command_ID_Map = [
    ("bsod", 0x0002),
    ("exit", 0x0005),
]

con = Console()
def Task_Engine(command: str) -> Dict:
    if command == "": return {"CmdID": 0x0000, "Args": ""}
    if command == "close":
        con.print('[bold blue][*][/] Ended session')
        return {
            "CmdID": 0xffff,
            "Args": ""
        }

    for index in Command_ID_Map:
        if command.split(' ', 1)[0] == index[0]:
            try:    
                return {
                    "CmdID": index[1],
                    "Args": command.split(' ', 1)[1]
                }
            except IndexError:
                return {
                    "CmdID": index[1],
                    "Args": ""
                }
    
    # * Run PowerShell by default
    return {
        "CmdID": 0x0001,
        "Args": command
    }
