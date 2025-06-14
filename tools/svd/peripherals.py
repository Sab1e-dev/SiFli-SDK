import sys

def get_peripheral_list(soc_name):
    periph_list = []
    with open(soc_name+'.txt') as f:
        for line in f:
            (name, base) = line.split(',')
            base = base.strip()
            name = name[0:name.find('_BASE')]
            if name[-1] >= '0' and name[-1] <= '9':
                group_name = name[0:-1]
            else:
                group_name = name
            if name == "IWDT":
                periph_list.append({'name': name, 'group_name': "WDT", 'base': base})
            else:    
                periph_list.append({'name': name, 'group_name': group_name, 'base': base})

    return periph_list        

