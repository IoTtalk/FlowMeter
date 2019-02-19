import DAN

ServerURL='https://DomainName'# for secure connection
Comm_interval = 15 # unit:second

def profile_init():
    DAN.profile['dm_name']='FlowMeter'
    DAN.profile['d_name']='FlowMeter1' #DAN.get_mac_addr()[-4:]

def odf():  # int only
    return []

def idf():
    return [
       ('Waterflow', float),
    ]
