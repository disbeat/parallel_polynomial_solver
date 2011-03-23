1 - Para compilar os programas basta ir ao ficheiro makefile e alterar a variável "CC" para o compilador pretendido, gravar, e depois executar o comando "make" na consola.

Nota: O compilador default é o gcc, mas se a versão presente no sistema for inferior a 4.2.1, não é possível compilar, pois as versões anteriores não têm compatibilidade com openMP.

	
2 - Para executar o programa bastará escrever na consola ./[nome_executável]






Em anexo segue ainda um script python, com o nome "run_tests.py", que permite correr testes que estejam dentro de uma determinada pasta, e depois construir um gráfico com os resultados obtidos.

Para correr este script bastará fazer: python run_tests.py run_tests [pasta_com_os_ficheiro_de_input]

Notas:
	- [pasta_com_os_ficheiro_de_input] - esta pasta deverá conter apenas ficheiros de input, e cujo o nome deverá ter o seguinte formato: "input[grau_polinomio_[num_inputs].txt]"
	
	- Este script tem algumas depêndencias, nomeadamente da biblioteca matplotlib. Não é garantido que os gráficos sejam apresentados se esta biblioteca não estiver instalada no sistema.

