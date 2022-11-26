
# 실행환경 : Oracle VM VirtualBox ubuntu

# 터미널 명령어

# 1. Producer consumer model의 race condition 확인

cd producer_consumer
gcc -o producers_consumers_racecondition_ver producers_consumers_racecondition_ver.c
./producers_consumers_racecondition_ver

# 2. 위 과정에 이어 동기화된 producer consumer model 실행

gcc -o producers_consumers producers_consumers.c
./producers_consumers

# 3. 위 과정에 이어writer reader model의 race condition 확인

cd ..
cd writer_reader
gcc -o writers_readers_racecondition_ver writers_readers_racecondition_ver.c
./writers_readers_raceconditon

# 4. 위 과정에 이어 동기화 된 writer reader model 실행

gcc -o writers_readers writers_readers.c
./writers_readers