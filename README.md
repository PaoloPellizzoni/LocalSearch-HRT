# LocalSearch-HRT

The _Stable Marriage_ problem consists in matching elements of two distinct sets of the same cardinality, usually referred to as men and women. Each element of the two sets is endowed with a (strict) ranking of all the elements of the other set (i.e. all men rank the women and vice versa). In broad terms, we want to find a matching such that no pair of man and woman would leave his and her respective partners to form a marriage among themselves (blocking pair).

In the _Hospital-Residents problem with ties (HRT)_ one side of the market, which is usually referred to as hospitals, is allowed to accept multiple partners up to a predefined individual capacity. Elements of the other set, which is usually called resident set and does not have to be of the same cardinality ot the other, is conversely allowed to have a single partner. In this variant of the problem each resident ranks the hospitals he finds _acceptable_. Similarly, each hospital ranks all the residents that find it acceptable. Moreover, we allow the preference lists to contain ties, hence the ranking is not strict anymore. A matching _M_ is said to be weakly stable if it has no blocking pairs.

Given a HRT instance, it is NP-hard to find the stable matching of maximal size, hence we developed state-of-the-art local search methods in order to solve even fairly big instances.
