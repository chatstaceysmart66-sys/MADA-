#!/usr/bin/env bash
# Builds and runs engine-independent host tests. Exit code is propagated (no fake success).
set -uo pipefail
cd "$(dirname "$0")/.."
mkdir -p Docs/logs
LOG=Docs/logs/host_tests.log
{
  echo "date_utc=$(date -u +%FT%TZ)"; echo "compiler=$(g++ --version | head -1)"; echo "commit=$(git rev-parse --short HEAD 2>/dev/null || echo none)"
  echo "cmd: g++ -std=c++17 -Wall -Wextra -Werror -O1 Tests/HostTests/test_main.cpp -o /tmp/ow_host_tests"
  g++ -std=c++17 -Wall -Wextra -Werror -O1 Tests/HostTests/test_main.cpp -o /tmp/ow_host_tests
  RC=$?; echo "compile_exit=$RC"; [ $RC -ne 0 ] && exit $RC
  /tmp/ow_host_tests; RC=$?; echo "run_exit=$RC"; exit $RC
} 2>&1 | tee "$LOG"
exit "${PIPESTATUS[0]}"
