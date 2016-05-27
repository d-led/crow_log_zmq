#include "resources.h"

std::string resources::index_html = R"(
Logs
====

{{#logs}}
- [{{filename}}]({{url}}) [{{size}}]
{{/logs}}

<style>h1:before, h2:before { content: none; }</style>
<!-- Markdeep: --><style class="fallback">body{visibility:hidden;white-space:pre;font-family:monospace}</style><script src="markdeep.min.js"></script><script src="https://casual-effects.com/markdeep/latest/markdeep.min.js"></script><script>window.alreadyProcessedMarkdeep||(document.body.style.visibility="visible")</script>
)";
