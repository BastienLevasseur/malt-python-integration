/********************  HEADERS  *********************/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <common/GTestHelper.hpp>

#include <python-integration/MaltPythonAllocator.hpp>
#include <python-integration/PythonAllocatorDomain.hpp>
#include <python-integration/PythonInterpreterStarter.hpp>
#include <common/Debug.hpp>


using namespace MALT;
using namespace testing;

static MaltPythonAllocator_t* gblMaltPythonAllocator = nullptr;
static DummyStatistics* gblDummyStats = nullptr;

static void* getAllocationContext(PythonAllocatorDomain pythonDomain){
	switch (pythonDomain){
		case PYMEM_DOMAIN_RAW:
			return gblMaltPythonAllocator->originalAllocator->allocatorRaw->ctx;
		case PYMEM_DOMAIN_MEM:
			return gblMaltPythonAllocator->originalAllocator->allocatorMem->ctx;
		case PYMEM_DOMAIN_OBJ:
			return gblMaltPythonAllocator->originalAllocator->allocatorObj->ctx;
		default:
			MALT_FATAL_ARG("Invalid Python allocator domain : %1").arg(pythonDomain).end();
	}
}


static void makeMalloc(PythonAllocatorDomain pythonDomain, size_t mallocSize){
	void* allocedPtr = nullptr;
	void* context = getAllocationContext(pythonDomain);
	
	switch (pythonDomain){
		case PYMEM_DOMAIN_RAW:		
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorRaw->malloc(context, mallocSize);
			break;
		case PYMEM_DOMAIN_MEM:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorMem->malloc(context, mallocSize);
			break;
		case PYMEM_DOMAIN_OBJ:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorObj->malloc(context, mallocSize);
			break;
		default:
			MALT_FATAL_ARG("Invalid Python allocator domain : %1").arg(pythonDomain).end();
	}
	assert(allocedPtr != nullptr);
	free(allocedPtr);
}


static void makeCalloc(PythonAllocatorDomain pythonDomain, size_t nbElement, size_t elementSize){
	void* allocedPtr = nullptr;
	void* context = getAllocationContext(pythonDomain);

	switch (pythonDomain){
		case PYMEM_DOMAIN_RAW:	
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorRaw->calloc(context, nbElement, elementSize);
			break;
		case PYMEM_DOMAIN_MEM:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorMem->calloc(context, nbElement, elementSize);
			break;
		case PYMEM_DOMAIN_OBJ:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorObj->calloc(context, nbElement, elementSize);
			break;
		default:
			MALT_FATAL_ARG("Invalid Python allocator domain : %1").arg(pythonDomain).end();
	}
	free(allocedPtr);
}


static void makeRealloc(PythonAllocatorDomain pythonDomain, void* allocPtr, size_t newSize){
	void* allocedPtr = nullptr;
	void* context = getAllocationContext(pythonDomain);

	switch (pythonDomain){
		case PYMEM_DOMAIN_RAW:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorRaw->realloc(context, allocPtr, newSize);
			break;
		case PYMEM_DOMAIN_MEM:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorMem->realloc(context, allocPtr, newSize);
			break;
		case PYMEM_DOMAIN_OBJ:
			allocedPtr = gblMaltPythonAllocator->customAllocator->allocatorObj->realloc(context, allocPtr, newSize);
			break;
		default:
			MALT_FATAL_ARG("Invalid Python allocator domain : %1").arg(pythonDomain).end();
	}
	free(allocedPtr);
}



TEST(TestPythonNullSizes, TestRawMalloc){
	makeMalloc(PYMEM_DOMAIN_RAW, 0);

	size_t mallocRawSum = gblDummyStats->getMallocSum(PYMEM_DOMAIN_RAW);
	EXPECT_EQ(mallocRawSum, 1);
}

TEST(TestPythonNullSizes, TestMemMalloc){
	makeMalloc(PYMEM_DOMAIN_MEM, 0);

	size_t mallocMemSum = gblDummyStats->getMallocSum(PYMEM_DOMAIN_MEM);
	EXPECT_EQ(mallocMemSum, 1);
}

TEST(TestPythonNullSizes, TestObjMalloc){
	makeMalloc(PYMEM_DOMAIN_OBJ, 0);

	size_t mallocObjSum = gblDummyStats->getMallocSum(PYMEM_DOMAIN_OBJ);
	EXPECT_EQ(mallocObjSum, 1);
}



TEST(TestPythonNullSizes, TestRawCalloc){
	makeCalloc(PYMEM_DOMAIN_RAW, 0, 1);

	size_t callocRawSum = gblDummyStats->getCallocSum(PYMEM_DOMAIN_RAW);
	EXPECT_EQ(callocRawSum, 1);
}

TEST(TestPythonNullSizes, TestMemCalloc){
	makeCalloc(PYMEM_DOMAIN_MEM, 0, 1);

	size_t callocMemSum = gblDummyStats->getCallocSum(PYMEM_DOMAIN_MEM);
	EXPECT_EQ(callocMemSum, 1);
}

TEST(TestPythonNullSizes, TestObjCalloc){
	makeCalloc(PYMEM_DOMAIN_OBJ, 0, 1);

	size_t callocObjSum = gblDummyStats->getCallocSum(PYMEM_DOMAIN_OBJ);
	EXPECT_EQ(callocObjSum, 1);
}



TEST(TestPythonNullSizes, TestRawRealloc){
	void* allocedPtr = malloc(1);
	makeRealloc(PYMEM_DOMAIN_RAW, allocedPtr, 0);

	size_t reallocRawSum = gblDummyStats->getReallocCount(PYMEM_DOMAIN_RAW);
	EXPECT_EQ(reallocRawSum, 1);
}

TEST(TestPythonNullSizes, TestMemRealloc){
	void* allocedPtr = malloc(1);
	makeRealloc(PYMEM_DOMAIN_MEM, allocedPtr, 0);

	size_t reallocMemSum = gblDummyStats->getReallocCount(PYMEM_DOMAIN_MEM);
	EXPECT_EQ(reallocMemSum, 1);
}

TEST(TestPythonNullSizes, TestObjRealloc){
	void* allocedPtr = malloc(1);
	makeRealloc(PYMEM_DOMAIN_OBJ, allocedPtr, 0);

	size_t reallocObjSum = gblDummyStats->getReallocCount(PYMEM_DOMAIN_OBJ);
	EXPECT_EQ(reallocObjSum, 1);
}



int main(int argc, char** argv){
	gblDummyStats = new DummyStatistics();
	PythonHandler* pythonHandler = new PythonHandler(gblDummyStats);

	PythonInterpreterStarter::startPythonInterpreter(0, nullptr);

	initialiseMaltPythonAllocator(pythonHandler);
	gblMaltPythonAllocator = getGlobalMaltPythonAllocator();
	enableMaltPythonAllocator(gblMaltPythonAllocator);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}