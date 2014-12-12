#!/usr/bin/env python

from csv import reader
from csv import writer

# csv format
#    0           1           2
# freq         word1        word2

data_file = 'w2_.txt'
total = 0
the = 0
with open(data_file, 'r') as old_data:
    data_base = reader(old_data, delimiter='\t')
    for record in data_base:
        record_freq = int(record[0])
        total += record_freq
        if record[1] == 'the':
            the += record_freq
        if record[1] == 'the' and record[2] == 'agent':
            the_agent = int(record[0])
        if record[1] == 'agent' and record[2] == 'has':
            agent_has = int(record[0])
        if record[1] == 'has' and record[2] == 'the':
            has_the = int(record[0])
        if record[1] == 'the' and record[2] == 'gold':
            the_gold = int(record[0])
        if record[1] == 'has' and record[2] == 'an':
            has_an = int(record[0])
        if record[1] == 'an' and record[2] == 'arrow':
            an_arrow = int(record[0])
print 'total =', total
print 'the =', float(the)/float(total)
print 'the_agent =', float(the_agent)/float(total)
print 'agent_has =', float(agent_has)/float(total)
print 'has_the =', float(has_the)/float(total)
print 'the_gold =', float(the_gold)/float(total)
print 'has_an =', float(has_an)/float(total)
print 'an_arrow =', float(an_arrow)/float(total)
        # if thread_count == 1:
        #     singlethread_runtime = float(record[4])
        #     speedup = 1
        # else:
        #     multithread_runtime = float(record[4])
        #     speedup = singlethread_runtime / multithread_runtime
        # record.append(' ' + str(speedup))
        # data_writer.writerow(record)
