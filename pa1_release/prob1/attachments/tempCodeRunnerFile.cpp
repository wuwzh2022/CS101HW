
static_assert(container_checks::Correct<Slist>);

int main() {
  std::cout << "Compile-time checks passed." << std::endl;
  return 0;
}