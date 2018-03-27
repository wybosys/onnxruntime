#include "core/providers/cpu/math/element_wise_ops.h"
#include "gtest/gtest.h"
#include "test/test_utils.h"

namespace Lotus {
namespace Test {

static const TypeProto_Set s_typeProto_float{TensorProto_DataType_FLOAT};

TEST(MathOpTest, Add) {
  LotusIR::NodeArg input1_def("A", &s_typeProto_float), input2_def("B", &s_typeProto_float), output_def("C", &s_typeProto_float);
  TestModel model("Add", {&input1_def, &input2_def}, {&output_def});
  SimpleFloatTest<Add> test(model);

  std::vector<int64_t> dims{3, 3};
  test.AddInput(dims, {1.0f, 2.0f, -1.0f, 0.0f, 1.5f, -100.0f, -5.4f, 9.3f, -10'000.0f});
  test.AddInput(dims, {-1.0f, 4.4f, 432.3f, 0.0f, 3.5f, 64.0f, -5.4f, 9.3f, 10'000.0f});
  test.AddOutput(dims);
  float expected_vals[]{0.0f, 6.4f, 431.3f, 0.0f, 5.0f, -36.0f, -10.8f, 18.6f, 0.0f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Add_Broadcast_Axis) {
  LotusIR::NodeArg input1_def("A", &s_typeProto_float), input2_def("B", &s_typeProto_float), output_def("C", &s_typeProto_float);
  TestModel model("Add", {&input1_def, &input2_def}, {&output_def});

  EXPECT_TRUE(model.Node().AddAttribute("axis", int64_t{0}));
  EXPECT_TRUE(model.Node().AddAttribute("broadcast", int64_t{1}));

  SimpleFloatTest<Add> test(model);

  std::vector<int64_t> dims{3, 3};
  test.AddInput(dims, {1.0f, 2.0f, 3.0f,
                       4.0f, 5.0f, 6.0f,
                       7.0f, 8.0f, 9.0f});
  test.AddInput({3}, {3.0f,
                      2.0f,
                      1.0f});
  test.AddOutput(dims);
  float expected_vals[]{4.0f, 5.0f, 6.0f,
                        6.0f, 7.0f, 8.0f,
                        8.0f, 9.0f, 10.0f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Add_Broadcast) {
  LotusIR::NodeArg input1_def("A", &s_typeProto_float), input2_def("B", &s_typeProto_float), output_def("C", &s_typeProto_float);
  TestModel model("Add", {&input1_def, &input2_def}, {&output_def});

  EXPECT_TRUE(model.Node().AddAttribute("broadcast", int64_t{1}));

  SimpleFloatTest<Add> test(model);

  std::vector<int64_t> dims{3, 2};
  test.AddInput(dims, {1.0f, 2.0f,
                       3.0f, 4.0f,
                       5.0f, 6.0f});
  test.AddInput({3}, {1.0f,
                      2.0f,
                      3.0f});
  test.AddOutput(dims);
  float expected_vals[]{2.0f, 3.0f,
                        5.0f, 6.0f,
                        8.0f, 9.0f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Sub) {
  LotusIR::NodeArg input1_def("A", &s_typeProto_float), input2_def("B", &s_typeProto_float), output_def("C", &s_typeProto_float);
  TestModel model("Sub", {&input1_def, &input2_def}, {&output_def});
  SimpleFloatTest<Sub> test(model);

  std::vector<int64_t> dims{3, 3};
  test.AddInput(dims, {1.0f, 2.0f, -1.0f, 0.0f, 1.5f, -100.0f, -5.4f, 9.3f, -10'000.0f});
  test.AddInput(dims, {-1.0f, 4.4f, 432.3f, 0.0f, 3.5f, 64.0f, -5.4f, 9.3f, 10'000.0f});
  test.AddOutput(dims);
  float expected_vals[]{2.0f, -2.4f, -433.3f, 0.0f, -2.0f, -164.0f, 0.0f, 0.0f, -20'000.0f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Mul) {
  LotusIR::NodeArg input1_def("A", &s_typeProto_float), input2_def("B", &s_typeProto_float), output_def("C", &s_typeProto_float);
  TestModel model("Mul", {&input1_def, &input2_def}, {&output_def});
  SimpleFloatTest<Mul> test(model);

  std::vector<int64_t> dims{3, 3};
  test.AddInput(dims, {1.0f, 2.0f, -1.0f, 0.0f, 1.5f, -100.0f, -5.4f, 9.30f, -10'000.0f});
  test.AddInput(dims, {-1.0f, 4.4f, 432.3f, 0.0f, 3.5f, 64.0f, -5.4f, 9.30f, 10'000.0f});
  test.AddOutput(dims);
  float expected_vals[]{-1.0f, 8.8f, -432.3f, 0.0f, 5.25f, -6'400.0f, 29.16f, 86.49f, -100'000'000.0f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Div) {
  LotusIR::NodeArg input1_def("A", &s_typeProto_float), input2_def("B", &s_typeProto_float), output_def("C", &s_typeProto_float);
  TestModel model("Div", {&input1_def, &input2_def}, {&output_def});
  SimpleFloatTest<Div> test(model);

  std::vector<int64_t> dims{2, 3};
  test.AddInput(dims, {1'000.0f, 1.0f, 6.0f,
                       0.0f, -10.0f, -1.0f});
  test.AddInput(dims, {1'000.0f, 2.0f, 3.0f,
                       1.0f, -1.0f, 4.0f});
  test.AddOutput(dims);
  float expected_vals[]{1.0f, 0.5f, 2.0f,
                        0.0f, 10.0f, -0.25f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Reciprocal) {
  LotusIR::NodeArg input_def("X", &s_typeProto_float), output_def("Y", &s_typeProto_float);
  TestModel model("Reciprocal", {&input_def}, {&output_def});
  SimpleFloatTest<Reciprocal> test(model);

  std::vector<int64_t> dims{2, 2};
  test.AddInput(dims, {1.0f, 2.0f, -1.0f, -2.0f});
  test.AddOutput(dims);
  float expected_vals[]{1.0f, 0.5f, -1.0f, -0.5f};
  test.Run(dims, expected_vals);
}

TEST(MathOpTest, Sum) {
  LotusIR::NodeArg input1_def("data_0", &s_typeProto_float), input2_def("data_1", &s_typeProto_float), input3_def("data_3", &s_typeProto_float), output_def("sum", &s_typeProto_float);
  TestModel model("Sum", {&input1_def, &input2_def, &input3_def}, {&output_def});
  SimpleFloatTest<Sum> test(model);

  std::vector<int64_t> dims{3, 3};
  test.AddInput(dims, {1.0f, 0.0f, 1.0f, -1.0f, 1.1f, -100.0f, -5.4f, 0.01f, -10'000.0f});
  test.AddInput(dims, {1.0f, 0.0f, 2.0f, -2.0f, 2.2f, 64.0f, -1.0f, 0.02f, 0.1f});
  test.AddInput(dims, {1.0f, 0.0f, 3.0f, -3.0f, 3.3f, 64.0f, 5.4f, 0.03f, 10'000.0f});
  test.AddOutput(dims);
  float expected_vals[]{3.0f, 0.0f, 6.0f, -6.0f, 6.6f, 28.0f, -1.0f, 0.06f, 0.1f};
  test.Run(dims, expected_vals);
}

}  // namespace Test
}  // namespace Lotus
