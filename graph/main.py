import matplotlib.pyplot as plt

PATH = "/home/blairi/development/C/merge-array-parallel/"


def get_data(path:str) -> tuple[list[int], list[float]]:

    file = open(path, "r")

    ns = list()
    times = list()
    for line in file:

        line = line.rstrip()
        if line == "":
            continue

        n = line.split(":")[0]
        time = line.split(":")[1]

        ns.append( int(n) )
        times.append( float(time) )

    file.close()

    return ( ns, times )


def main() -> None:

    n_serial, times_serial = get_data(f"{PATH}data/serial_time.txt")
    n_parallel, times_parallel = get_data(f"{PATH}data/parallel_time.txt")

    fig, ax = plt.subplots()

    ax.plot(n_serial, times_serial, label = 'Serial', marker = '*', color = 'b')
    ax.plot(n_parallel, times_parallel, label = 'Parallel', marker = '*', color = 'r')
    
    plt.title('Selection sort\nParallel vs Serial')
    ax.set_ylabel('Time (s)')
    ax.set_xlabel('Elements')
    ax.grid(True)
    ax.legend(loc=2)

    plt.show()


main()
