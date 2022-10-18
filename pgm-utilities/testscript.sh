#!/bin/sh
# testing script

# colours
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'


echo "=============  PGM Utilities Test Script  ============="

# ===========================================  pgmEcho tests  ============================================ #
echo -e "\nTesting pgmEcho:"

# test usage message
if [ "$(./pgmEcho)" = "Usage: ./pgmEcho inputImage.pgm outputImage.pgm" ]; then
	echo -e "pgmEcho: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgmEcho: USAGE - ${RED}FAILED${NC}"
fi

# test bad argument count
if [ "$(./pgmEcho a)" = "ERROR: Bad Argument Count" ]; then
    if [ "$(./pgmEcho a b c)" = "ERROR: Bad Argument Count" ]; then
	    echo -e "pgmEcho: BAD ARGUMENTS - ${GREEN}PASSED${NC}"
    else
	    echo -e "pgmEcho: BAD ARGUMENTS - ${RED}FAILED${NC}"
    fi
else
	echo -e "pgmEcho: BAD ARGUMENTS - ${RED}FAILED${NC}"
fi

# test success message
if [ "$(./pgmEcho testing/test_slice.pgm testing/test_slice2.pgm)" = "ECHOED" ]; then
	echo -e "pgmEcho: SUCESS OUTPUT - ${GREEN}PASSED${NC}"
else
	echo -e "pgmEcho: SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi

# test against expected (won't work if success fails)
if [ "$(cat testing/test_slice2.pgm)" = "$(cat testing/expected_echo.pgm)" ]; then
	echo -e "pgmEcho: ECHOES AS INTENDED - ${GREEN}PASSED${NC}"
else
	echo -e "pgmEcho: ECHOES AS INTENDED - ${RED}FAILED${NC}"
fi

# ===========================================  pgmComp tests  ============================================= #
echo -e "\nTesting pgmComp:"

# test usage message
if [ "$(./pgmComp)" = "Usage: ./pgmComp inputImage.pgm inputImage.pgm" ]; then
	echo -e "pgmComp: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgmComp: USAGE - ${RED}FAILED${NC}"
fi

# test bad argument count
if [ "$(./pgmComp a)" = "ERROR: Bad Argument Count" ]; then
    if [ "$(./pgmComp a b c)" = "ERROR: Bad Argument Count" ]; then
	    echo -e "pgmComp: BAD ARGUMENTS - ${GREEN}PASSED${NC}"
    else
	    echo -e "pgmComp: BAD ARGUMENTS - ${RED}FAILED${NC}"
    fi
else
	echo -e "pgmComp: BAD ARGUMENTS - ${RED}FAILED${NC}"
fi

# test success message 1
identical=0
if [ "$(./pgmComp testing/test_slice.pgm testing/test_slice_identical.pgm)" = "IDENTICAL" ]; then
	echo -e "pgmComp: IDENTICAL SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
    identical=1;
else
	echo -e "pgmComp: IDENTICAL SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi

# test success message 2
different=0;
if [ "$(./pgmComp testing/test_slice.pgm testing/test_slice_different.pgm)" = "DIFFERENT" ]; then
	echo -e "pgmComp: DIFFERENT SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
    different=1;
else
	echo -e "pgmComp: DIFFERENT SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi

# test both success messages
if [ $identical -eq 1 ] && [ $different -eq 1 ]; then
	echo -e "pgmComp: COMPARES AS INTENDED - ${GREEN}PASSED${NC}"
else
	echo -e "pgmComp: COMPARES AS INTENDED - ${RED}FAILED${NC}"
fi

# ===========================================  pgma2b tests  ============================================== #
echo -e "\nTesting pgma2b:"

# test usage message
if [ "$(./pgma2b)" = "Usage: ./pgma2b inputImage.pgm outputImage.pgm" ]; then
	echo -e "pgma2b: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgma2b: USAGE - ${RED}FAILED${NC}"
fi

# test bad argument count
if [ "$(./pgma2b a)" = "ERROR: Bad Argument Count" ]; then
    if [ "$(./pgma2b a b c)" = "ERROR: Bad Argument Count" ]; then
	    echo -e "pgma2b: BAD ARGUMENTS - ${GREEN}PASSED${NC}"
    else
	    echo -e "pgma2b: BAD ARGUMENTS - ${RED}FAILED${NC}"
    fi
else
	echo -e "pgma2b: BAD ARGUMENTS - ${RED}FAILED${NC}"
fi

# test success message
if [ "$(./pgma2b testing/test_slice.pgm testing/test_slice_a2b.pgm)" = "CONVERTED" ]; then
	echo -e "pgma2b: SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
else
	echo -e "pgma2b: SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi


# ===========================================  pgmb2a tests  ============================================== #
echo -e "\nTesting pgmb2a:"

# test usage message
if [ "$(./pgmb2a)" = "Usage: ./pgmb2a inputImage.pgm outputImage.pgm" ]; then
	echo -e "pgmb2a: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgmb2a: USAGE - ${RED}FAILED${NC}"
fi

# test bad argument count
if [ "$(./pgmb2a a)" = "ERROR: Bad Argument Count" ]; then
    if [ "$(./pgmb2a a b c)" = "ERROR: Bad Argument Count" ]; then
	    echo -e "pgmb2a: BAD ARGUMENTS - ${GREEN}PASSED${NC}"
    else
	    echo -e "pgmb2a: BAD ARGUMENTS - ${RED}FAILED${NC}"
    fi
else
	echo -e "pgmb2a: BAD ARGUMENTS - ${RED}FAILED${NC}"
fi

# test success message
if [ "$(./pgmb2a testing/test_slice_a2b.pgm testing/test_sliceb2a.pgm)" = "CONVERTED" ]; then
	echo -e "pgmb2a: SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
else
	echo -e "pgmb2a: SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi


# ===========================================  pgmReduce tests  =========================================== #
echo -e "\nTesting pgmReduce:"

# test usage message
if [ "$(./pgmReduce)" = "Usage: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm" ]; then
	echo -e "pgmReduce: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: USAGE - ${RED}FAILED${NC}"
fi

# test bad argument count
if [ "$(./pgmReduce a)" = "ERROR: Bad Argument Count" ]; then
    if [ "$(./pgmReduce a b)" = "ERROR: Bad Argument Count" ]; then
	    if [ "$(./pgmReduce a b c d)" = "ERROR: Bad Argument Count" ]; then
	        echo -e "pgmReduce: BAD ARGUMENTS - ${GREEN}PASSED${NC}"
        else
            echo -e "pgmReduce: BAD ARGUMENTS - ${RED}FAILED${NC}"
        fi
    else
	    echo -e "pgmReduce: BAD ARGUMENTS - ${RED}FAILED${NC}"
    fi
else
	echo "pgmReduce: BAD ARGUMENTS - ${RED}FAILED${NC}"
fi

# test success message
if [ "$(./pgmReduce testing/baboon.pgm 2 testing/reduce_try.pgm)" = "REDUCED" ]; then
	echo -e "pgmReduce: SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi

# test against expected (won't work if success fails)
if [ "$(./pgmComp testing/reduce_try.pgm testing/baboonsmol.pgm)" = "IDENTICAL" ]; then
	echo -e "pgmEcho: REDUCES AS INTENDED - ${GREEN}PASSED${NC}"
else
	echo -e "pgmEcho: REDUCES AS INTENDED - ${RED}FAILED${NC}"
fi

# ===========================================  pgmTile tests  =========================================== #
echo -e "\nTesting pgmTile:"

# test usage message
if [ "$(./pgmTile)" = "Usage: ./pgmTile inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm" ]; then
	echo -e "pgmTile: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgmTile: USAGE - ${RED}FAILED${NC}"
fi

# test success message
if [ "$(./pgmTile testing/baboon.pgm 2 "testing/test_baboontile_<row>_<column>.pgm")" = "TILED" ]; then
	echo -e "pgmReduce: SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi

# test against expected (won't work if success fails)
if [ "$(./pgmComp "testing/test_baboontile_0_0.pgm" "testing/baboontiled_0_0.pgm")" = "IDENTICAL" ]; then
	echo -e "pgmReduce: REDUCES AS INTENDED (0, 0) - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: REDUCES AS INTENDED (0, 0) - ${RED}FAILED${NC}"
fi

# test against expected (won't work if success fails)
if [ "$(./pgmComp "testing/test_baboontile_0_1.pgm" "testing/baboontiled_0_1.pgm")" = "IDENTICAL" ]; then
	echo -e "pgmReduce: REDUCES AS INTENDED (0, 1) - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: REDUCES AS INTENDED (0, 1) - ${RED}FAILED${NC}"
fi

# test against expected (won't work if success fails)
if [ "$(./pgmComp "testing/test_baboontile_1_0.pgm" "testing/baboontiled_1_0.pgm")" = "IDENTICAL" ]; then
	echo -e "pgmReduce: REDUCES AS INTENDED (1, 0) - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: REDUCES AS INTENDED (1, 0) - ${RED}FAILED${NC}"
fi

# test against expected (won't work if success fails)
if [ "$(./pgmComp "testing/test_baboontile_1_1.pgm" "testing/baboontiled_1_1.pgm")" = "IDENTICAL" ]; then
	echo -e "pgmReduce: REDUCES AS INTENDED (1, 1) - ${GREEN}PASSED${NC}"
else
	echo -e "pgmReduce: REDUCES AS INTENDED (1, 1) - ${RED}FAILED${NC}"
fi

# ===========================================  pgmAssemble tests  =========================================== #
echo -e "\nTesting pgmAssemble:"

# test usage message
if [ "$(./pgmAssemble)" = "Usage: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+" ]; then
	echo -e "pgmAssemble: USAGE - ${GREEN}PASSED${NC}"
else
	echo -e "pgmAssemble: USAGE - ${RED}FAILED${NC}"
fi

# test success message
if [ "$(./pgmAssemble testing/test_baboonassemble.pgm 512 512 0 0 testing/baboonassemble_0_0.pgm 0 256 testing/baboonassemble_0_1.pgm 256 0 testing/baboonassemble_1_0.pgm 256 256 testing/baboonassemble_0_0.pgm])" = "ASSEMBLED" ]; then
	echo -e "Assembled: SUCCESS OUTPUT - ${GREEN}PASSED${NC}"
else
	echo -e "pgmAssembled: SUCCESS OUTPUT - ${RED}FAILED${NC}"
fi
