with open('tmp.txt', 'r') as f:
    lines = list(f.readlines())
constructs = [l for l in lines if 'CONSTRUCT' in l]
destructs = [l for l in lines if 'DESTRUCT' in l]
con_addrs = [l.split()[3][:-1] for l in constructs]
des_addrs = [l.split()[3][:-1] for l in destructs]
con_addrs = list(set(con_addrs))
des_addrs = list(set(des_addrs))
missing = [l for l in con_addrs if l not in des_addrs]
for m in missing:
    for l in lines:
        if m in l:
            print(l)