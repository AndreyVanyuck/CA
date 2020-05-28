1) mkdir build
2) cd build/
3) cmake ../
4) make 

Для запуска гугол тестов:
	1) раскомментировать в main.cpp: 
    		// testing::InitGoogleTest(&argc,argv);
    		// RUN_ALL_TESTS();
	2) закомментировать остальной код main.cpp
	3) make
	4) ./riscv_sim

Для запуска ассемблерных тестов: ./test.sh build/riscv_sim 

