// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of the Milena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_MORPHO_WATERSHED_TOPOLOGICAL_HH
# define MLN_MORPHO_WATERSHED_TOPOLOGICAL_HH

/// \file
///
/// \brief Topological Watershed Transform (WST) algorithm.
///
/// Reference:
///   Michel Couprie, Laurent Najman and Gilles Bertrand.
///   Quasi-linear algorithms for the topological watershed. In:
///   Journal of Mathematical Imaging and Vision (JMIV), vol. 22,
///   no. 2-3, pages 231--249, 2005.

# include <vector>
# include <map>
# include <queue>

# include <mln/core/site_set/p_set.hh>
# include <mln/core/site_set/p_priority.hh>
# include <mln/core/site_set/p_queue_fast.hh>

# include <mln/util/ord.hh>

# include <mln/data/sort_psites.hh>
# include <mln/data/fill.hh>


// FIXME: Clean up and document.

namespace mln
{

  namespace morpho
  {

    namespace watershed
    {

      template <class I>
      mln_site(I) min (const Image<I> &ima_, p_set<mln_site(I)>& components)
      {
	const I& ima = exact(ima_);

	if (components.nsites() == 0)
	  return mln_site(I)(-1, -1); // FIXME

	typename p_set<mln_site(I)>::fwd_piter it(components);
	mln_site(I) min = components[0];

	for_all(it)
	  if (ima(it) < ima(min))
	    min = it;

	return min;
      }

      template <class I>
      mln_site(I) max (p_set<mln_site(I)>& components)
      {
	if (components.nsites() == 0)
	  return mln_site(I)(-1, -1); // FIXME

	typename p_set<mln_site(I)>::fwd_piter it(components);
	mln_site(I) max = components[0];

	for_all(it)
	  if (ima(it) > ima(max))
	    max = it;

	return max;
      }


      // Actually, this structure is a tree, despite its confusing name.
      template <class I, class N>
      struct topo_wst
      {
	// Typedefs
	// --------
	typedef mln_site(I) site;
	typedef I image_t;
	typedef N neighborhood_t;

	// Component tree management
	// -------------------------
      private:
	struct node {
	  mln_value(I) level;
	  int area;
	  int highest;
	  typedef mln_site(I) site;
	  // Can't modify the sites in a p_array
	  // p_array<mln_site(I)> children;
	  std::vector<site> children;

	  void addChildren(const node& n)
	  {
	    // typename p_array<mln_site(I)>::fwd_piter it(n.children);
	    // for (it.start();
	    //      it.is_valid();
	    //      it.next())
	    //  children.append(it.to_site());
	    for (unsigned i=0; i < n.children.size(); ++i)
	      children.push_back(n.children[i]);
	  }

	  void addChild(const site p)
	  {
	    // children.append(n);
	    children.push_back(p);
	  }
	}; // struct node

      public:

	mln_ch_value(I, site) Par_node;
	mln_ch_value(I, site) Par_tree;

	mln_ch_value(I, int) Rnk_tree;
	mln_ch_value(I, int) Rnk_node;
	mln_ch_value(I, site) subtreeRoot;
	mln_ch_value(I, node) nodes;
	site Root;

      private:

	void MakeSet_tree(site x);
	void MakeSet_node(site x);
	site Find_tree(site x);
	site Find_node(site x);
	void BuildComponentTree();
	site MergeNode(site& node1, site& node2);
	site Link_tree(site x, site y);
	site Link_node(site x, site y);
	node MakeNode(int level);

      private:
	mln_ch_value(I, bool) isproc;

	// Ctor
      public:
	topo_wst(const Image<I>& i,
		 const Neighborhood<N>& n);

      public:
	const I &ima;
	const N &nbh;

      public:
	void go();

      private:
	site min (p_set<site>& components);
	site max (p_set<site>& components);
	bool highest_fork (p_set<site>& components);
	bool highest_fork (p_set<site>& components, site &r);

	// Optimized LCA Algorithm

      public:
	site lca (site a, site b);

      private:
	int *euler;
	int *depth;
	int ctree_size;
	std::map<site, int, mln::util::ord<site> > pos;
	site *repr;

	int *minim;
	int **Minim;

	void compute_ctree_size (site p);
	void build_euler_tour_rec(site p, int &position, int d);
	void build_euler_tour();
	void build_minim ();
	void removeOneSonNodes(site *p, mln_ch_value(I, site) &newPar_node);
	void compressTree();
      };

      /// Compute a toological watershed transform from \a tree.
      template <class T>
      typename T::image_t
      topological(T &tree);



# ifndef MLN_INCLUDE_ONLY

      // Ctor
      template <class I, class N>
      topo_wst<I, N>::topo_wst(const Image<I>& i,
			       const Neighborhood<N>& n)
	: ima(exact(i)), nbh(exact(n))
      {
	initialize(Par_node, i);
	initialize(Par_tree, i);
	initialize(Rnk_tree, i);
	initialize(Rnk_node, i);
	initialize(subtreeRoot, i);
	initialize(nodes, i);
	initialize(isproc, i);
      }

      template <class I, class N>
      void topo_wst<I, N>::MakeSet_tree(site x)
      {
	Par_tree(x) = x;
	Rnk_tree(x) = 0;
      }

      template <class I, class N>
      void topo_wst<I, N>::MakeSet_node(site x)
      {
	Par_node(x) = x;
	Rnk_node(x) = 0;
      }

      template <class I, class N>
      typename topo_wst<I, N>::site topo_wst<I, N>::Find_tree(site x)
      {
	if (Par_tree(x) != x)
	  Par_tree(x) = Find_tree(Par_tree(x));
	return Par_tree(x);
      }

      template <class I, class N>
      typename topo_wst<I, N>::site topo_wst<I, N>::Find_node(site x)
      {
	if (Par_node(x) != x)
	  Par_node(x) = Find_node(Par_node(x));
	return Par_node(x);
      }

      template <class I, class N>
      void topo_wst<I, N>::go()
      {
	BuildComponentTree();
	compressTree();

	build_euler_tour();
	build_minim();
      }

      template <class I, class N>
      void topo_wst<I, N>::BuildComponentTree()
      {
	// Init:

	// Sort the sites in increasing order
	p_array<mln_site(I)> S;
	S = data::sort_psites_increasing(ima);

	// Clear the marker map
	data::fill(isproc, false);
	for (int ip = 0; ip < int(S.nsites()); ++ip)
	  {
	    site p = S[ip];
	    MakeSet_node(p);
	    MakeSet_tree(p);
	    //	      if (subtreeRoot.hold(p))
	    subtreeRoot(p) = p;
	    //	      if (nodes.hold(p))
	    nodes(p) = MakeNode(ima(p));
	  }



	typename p_array<mln_site(I)>::fwd_piter ip(S);
	for_all(ip)
	{
	  site p = ip.to_site();

	  site curCanonicalElt = Find_tree(p);
	  site curNode = Find_node(subtreeRoot(curCanonicalElt));

	  // FIXME: Should be `n' instead of `q'.
	  mln_niter(N) q(nbh, ip);
	  for_all(q)
	    if (ima.has(q) and isproc(q) and ima(q) <= ima(p))
	      {
		site adjCanonicalElt = Find_tree(q);
		site adjNode = Find_node(subtreeRoot(adjCanonicalElt));
		if (curNode != adjNode)
		  {
		    if (nodes(curNode).level == nodes(adjNode).level)
		      curNode = MergeNode(adjNode, curNode);
		    else
		      {
			nodes(curNode).addChild(adjNode);
			nodes(curNode).area += nodes(adjNode).area;
			nodes(curNode).highest += nodes(adjNode).highest;
		      }
		  }

		curCanonicalElt = Link_tree(adjCanonicalElt, curCanonicalElt);
		subtreeRoot(curCanonicalElt) = curNode;
	      }
	  isproc(p) = true;
	}
	// Pour garder une map de correspondance site <-> local_root
	//            for (int ip = 0; ip < int(S.size()); ++ip)
	//              {
	//                site p = S[ip];
	//	       M(p) = Find_node(p);
	//	     }

	mln_piter(I) r(Par_node.domain());
	for_all(r)
	  Par_node(r) = Find_node(r);

	// Find the ``first'' site of ima, according to the forward
	// traversal order.
	mln_fwd_piter(I) rp(Par_node.domain());;
	rp.start();

	Root = subtreeRoot(Find_tree(Find_node(rp)));
      }


      template <class I, class N>
      typename topo_wst<I, N>::site topo_wst<I, N>::MergeNode(site& node1,
							      site& node2)
      {
	site tmpNode = Link_node(node1, node2);
	site tmpNode2;
	if (tmpNode == node2)
	  {
	    nodes(node2).addChildren(nodes(node1));
	    tmpNode2 = node1;
	  }
	else
	  {
	    nodes(node1).addChildren(nodes(node2));
	    tmpNode2 = node2;
	  }
	nodes(tmpNode).area += nodes(tmpNode2).area;
	nodes(tmpNode).highest += nodes(tmpNode2).highest;
	return tmpNode;
      }

      template <class I, class N>
      typename topo_wst<I, N>::site topo_wst<I, N>::Link_tree(site x, site y)
      {
	if (Rnk_tree(x) > Rnk_tree(y))
	  std::swap(x, y);
	else
	  if (Rnk_tree(x) == Rnk_tree(y))
	    Rnk_tree(y) += 1;
	Par_tree(x) = y;
	return y;
      }

      template <class I, class N>
      typename topo_wst<I, N>::site topo_wst<I, N>::Link_node(site x, site y)
      {
	if (Rnk_node(x) > Rnk_node(y))
	  std::swap(x, y);
	else
	  if (Rnk_node(x) == Rnk_node(y))
	    Rnk_node(y) += 1;
	Par_node(x) = y;
	return y;
      }

      template <class I, class N>
      typename topo_wst<I, N>::node topo_wst<I, N>::MakeNode(int level)
      {
	node n;
	n.level = level;
	n.area = 1;
	n.highest = level;
	return n;
      }


      template <class I, class N>
      bool topo_wst<I, N>::highest_fork (p_set<site>& components, site &r)
      {
	if (components.nsites() == 0)
	  {
	    std::cerr << "highest fork : empty set" << std::endl;
	    return false;
	  }

	site
	  m = min(components),
	  hfork = m;

	typename p_set<site>::fwd_piter it(components);
	for_all(it)
	{
	  // Can't remove the site from the set
	  if (it.to_site() == m)
	    continue;

	  site c = lca(hfork, it.to_site());
	  if (c != it.to_site())
	    hfork = c;
	}

	if (nodes(m).level == nodes(hfork).level)
	  return false;

	r = hfork;
	return true;
      }

      template <class I, class N>
      bool topo_wst<I, N>::highest_fork (p_set<site>& components) {
	site i;
	return highest_fork(components, i);
      }

      template <class I, class N>
      void topo_wst<I, N>::compute_ctree_size (site p)
      {
	ctree_size += 1;
	node& n = nodes(p);

	// typename p_array<mln_site(I)>::fwd_piter it(n.children);
	// for_all(it)
	//   compute_ctree_size(it.to_site());

	for (unsigned i=0; i < n.children.size(); ++i)
	  compute_ctree_size(n.children[i]);
      }


      template <class I, class N>
      void topo_wst<I, N>::build_euler_tour_rec(site p, int &position, int d)
      {
	if (pos.find(p) == pos.end())
	  pos[p] = position;

	repr[position] = p;
	depth[position] = d;
	euler[position] = pos[p];
	++position;
	node& n = nodes(p);

	// typename p_array<mln_site(I)>::fwd_piter it(n.children);
	// for_all(it)
	// {
	//   build_euler_tour_rec(it.to_site(), position, d+1);
	//   depth[position] = d; // Not optimized
	//   euler[position] = pos[p];
	//   repr[position] = p; // Pas necessaire?
	//   ++position;
	// }

	for (unsigned i=0; i < n.children.size(); ++i)
	  {
	    build_euler_tour_rec(n.children[i], position, d+1);
	    depth[position] = d; // Not optimized
	    euler[position] = pos[p];
	    repr[position] = p; // Pas necessaire?
	    ++position;
	  }
      }


      template <class I, class N>
      void topo_wst<I, N>::build_euler_tour ()
      {
	ctree_size = 0;
	compute_ctree_size(Root);

	int size = 2 * ctree_size - 1;

	// FIXME : free this
	euler = new int[size];
	depth = new int[size];
	repr = new site[size];

	int position = 0;
	build_euler_tour_rec(Root, position, 0);
      }


      template <class I, class N>
      void topo_wst<I, N>::build_minim ()
      {
	// compute_tree_size(); // Already done in build_euler_tour
	int size = 2 * ctree_size - 1;
	int logn = (int)(ceil(log((double)(size))/log(2.0)));
	// minim.reserve(size);
	minim = new int [logn * size]; // FIXME : Think about freeing this
	Minim = new int* [logn];

	Minim[0] = minim;

	// Init
	for (int i = 0; i < size - 1; ++i)
	  if (depth[euler[i]] < depth[euler[i+1]]) {
	    Minim[0][i] = i;
	  } else {
	    Minim[0][i] = i+1;
	  }

	//	Minim[0][size - 1] = size - 1;

	int k;
	for (int j = 1; j < logn; j++) {
	  k = 1 << (j - 1);
	  Minim[j] = &minim[j * size];
	  for (int i = 0; i < size; i++) {
	    if ((i + (k << 1)) >= size) {
	      //Minim[j][i] = size - 1;
	    }
	    else {
	      if (depth[euler[Minim[j - 1][i]]]
		  <= depth[euler[Minim[j - 1][i + k]]])
		Minim[j][i] = Minim[j - 1][i];
	      else
		Minim[j][i] = Minim[j - 1][i + k];
	    }
	  }
	}

      } // void build_minim ()


      template <class I, class N>
      typename topo_wst<I, N>::site topo_wst<I, N>::lca (site a, site b)
      {
	int
	  m = pos[a],
	  n = pos[b],
	  k;

	if (m == n)
	  return repr[m];

	if (m > n)
	  {
	    k = n;
	    n = m;
	    m = k;
	  }

	k = (int)(log((double)(n - m))/log(2.));

	if (depth[euler[Minim[k][m]]] < depth[euler[Minim[k][n - (1 << k)]]]) {
	  return repr[euler[Minim[k][m]]];
	} else {
	  return repr[euler[Minim[k][n - (1 << k)]]];
	}
      }


      template <class I, class N>
      void topo_wst<I, N>::removeOneSonNodes(site *p,
					     mln_ch_value(I, site) &newPar_node)
      {
	node &n = nodes(*p);

	if (n.children.size() == 1) // this node has 1 son, delete it
	  {
	    n.area = -1;
	    newPar_node(*p) = n.children[0];
	    *p = n.children[0];
	    removeOneSonNodes(p, newPar_node);
	  }
	else // there is more than one son, recursive call
	  {
	    for (unsigned i = 0; i < n.children.size(); ++i)
	      removeOneSonNodes(&(n.children[i]), newPar_node);
	  }
      }


      template <class I, class N>
      void topo_wst<I, N>::compressTree()
      {
	mln_ch_value(I, site) newPar_node;
	initialize(newPar_node, Par_node);

	// Remove the nodes with one son
	removeOneSonNodes(&Root, newPar_node);

	// Update the references on deleted nodes
	mln_piter(I) p(Par_node.domain());
	for_all(p)
	  while (nodes(Par_node(p)).area == -1)
	    Par_node(p) = newPar_node(Par_node(p));
      }

      template <class T>
      bool w_constructible(T &tree, typename T::site p, typename T::site &r)
      {

	typedef typename T::image_t I;
	typedef typename T::neighborhood_t N;

	const I &ima = exact(tree.ima);
	const N &nbh = exact(tree.nbh);

	// FIXME: Should be `n' instead of `q'.
	mln_niter(N) q(nbh, p);
	p_set<mln_site(I)> v;

	for_all(q)
	  // FIXME: Shouldn't it be: `ima.has(q)' instead of
	  // `ima.domain().has(q)'?
	  if (ima.domain().has(q) && ima(q) < ima(p))
	    v.insert(tree.Par_node(q));

	if (v.nsites() == 0)
	  return false;

	if (v.nsites() == 1)
	  {
	    r = v[0];
	    return true;
	  }

	mln_site(I) c = min(ima, v);
	mln_site(I) cmin = c;

	typename p_set<mln_site(I)>::fwd_piter it(v);
	for_all(it)
	{
	  // Can't remove the site from the set
	  if (it.to_site() == cmin)
	    continue;

	  mln_site(I) c1 = tree.lca(c, it);

	  if (c1 != it)
	    c = c1;
	}

	if (tree.nodes(c).level >= ima(p))
	  return false;

	r = c;
	return true;
      }

      template <class T>
      bool w_constructible(T &tree, typename T::site p) {
	typename T::site r;
	return w_constructible(tree, p, r);
      }

 

      template <class T>
      typename T::image_t topological(T &tree)
      {

	typedef typename T::image_t I;
	typedef typename T::neighborhood_t N;

	I ima = exact(tree.ima);
	const N &nbh = exact(tree.nbh);

	// Maxima components
	mln_ch_value(I, bool) cmax;
	initialize(cmax, ima);

	// Mark enqueued sites
	mln_ch_value(I, bool) enqueued;
	initialize(enqueued, ima);

	p_priority< mln_value(I), p_queue_fast<mln_site(I)> > l;
	// p_queue < site > m;
	std::queue<mln_site(I)> m;
	mln_value(I) min = mln_min(mln_value(I));

	std::cout << "Init" << std::endl;

	// Flag C-maxima
	data::fill(cmax, false);
	data::fill(enqueued, false);

	mln_piter(I) it(tree.Par_node.domain());
	for_all(it)
	  // if (nodes(Par_node(it.to_site())).children.nsites() == 0)
	  if (tree.nodes(tree.Par_node(it)).children.size() == 0)
	    {
	      cmax(it) = true;
	      m.push(it);
	    }

	while (!m.empty())
	  {
	    // FIXME: Should be `n' instead of `q'.
	    mln_niter(N) q(nbh, m.front());
	    // FIXME: Shouldn't it be: `cmax.has(q)' instead of
	    // `cmax.domain().has(q)'?
	    for_all(q)
	      if (cmax.domain().has(q) && !cmax(q) && !enqueued(q))
		{
		  enqueued(q) = true;
		  l.push(mln_max(mln_value(I)) - ima(q), q);
		}
	    m.pop();
	  }


	std::cout << "end" << std::endl;

	// Main loop
	while (!l.is_empty())
	  {
	    mln_site(I) x = l.front();
	    l.pop();
	    enqueued(x) = false;

	    mln_site(I) c;
	    bool is_w = w_constructible(tree, x, c);

	    if (is_w)
	      {
		ima(x) = tree.nodes(c).level;
		tree.Par_node(x) = c;

		// if (nodes(c).children.nsites() == 0)
		if (tree.nodes(c).children.size() == 0)
		  cmax(x) = true;
		else
		  // if (nodes(c).children.nsites() > 1)
		  if (tree.nodes(c).children.size() == 1)
		    std::cerr << "ERREUR COMPOSANTE BRANCHE "
			      << tree.nodes(c).children.size() << std::endl;


		// FIXME: Should be `n' instead of `q'.
		mln_niter(N) q(nbh, x);
		// FIXME: Shouldn't it be: `ima.has(q)' instead of
		// `ima.domain().has(q)'?
		for_all(q)
		  if (ima.domain().has(q) && !cmax(q) && !enqueued(q))
		    {
		      enqueued(q) = true;
		      l.push(mln_max(mln_value(I)) - ima(q), q); // FIXME:
		      // Just
		      // invert
		      // the
		      // priority.
		    }
	      } 
	  } // while(!l.empty())

	for_all(it)
	  ima(it) = tree.nodes(tree.Par_node(it)).level;

	return ima;
      }

# endif // MLN_INCLUDE_ONLY


    } // end of namespace mln::morpho::watershed

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_WATERSHED_TOPOLOGICAL_HH
