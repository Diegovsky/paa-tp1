import subprocess
from statistics import mean

input_file = 'test_input.txt'

class Times:
    def __init__(self, name) -> None:
        self.name = name
        self.sys = []
        self.user = []
        self.phys = []

    def push(self, sys, user, phys):
        self.sys.append(sys)
        self.user.append(user)
        self.phys.append(phys)

    def avg(self):
        sys, user, phys = tuple(mean(t) for t in [self.sys, self.user, self.phys])
        print(self.name + ':')
        print(f'Sys: {sys}')
        print(f'User: {user}')
        print(f'Phys: {phys}')


iotimes = Times('Io times')
runtimes = Times('Algorithm times')

def run_program(input_file):
    result = subprocess.run(['./tp1', '-i', input_file], capture_output=True)

    lines = [float(line.strip()) for line in result.stdout.split()]

    iotimes.push(*lines[:3])
    runtimes.push(*lines[3:])

run_count = 20
for i in range(run_count):
    run_program(input_file)


iotimes.avg()
runtimes.avg()
