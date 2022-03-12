# modifique o valor de -divide_by na ultimalinha para ficar igual
# ao valor do parametro divide_by do seu gerador de clock no arquivo .sv
create_clock -name CLOCK_50 -period 20 [get_ports CLOCK_50]
create_generated_clock -name CLOCK_DIV -divide_by 250000000 -source CLOCK_50 [get_nets CLOCK_DIV]

