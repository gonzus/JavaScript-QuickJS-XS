JavaScript::QuickJS::XS - Perl XS binding for the
[QuickJS](https://bellard.org/quickjs/quickjs.html) Javascript engine.

This is a work in progress. I could use help with the following questions:

* Dealing with `JS` **arrays** from `C`:
  * Querying array size.
  * Fetching elements given their position.
  * Creating a new (empty) array.
  * Adding elements to the array.
  * Removing elements from the array.
* Dealing with `JS` **objects** from `C`:
  * Querying all the existing property names.
  * Fetching properties given their name.
  * Creating a new (empty) object.
  * Adding properties to the object.
  * Removing properties from the object.
* Dealing with `JS` **timers** from `C`:
  * Creating a new timer.
  * Dispatching all pending timers.
