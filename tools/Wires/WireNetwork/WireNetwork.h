#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Core/EigenTypedef.h>
#include <Wires/Attributes/WireAttributes.h>
#include "WireConnectivity.h"

class WireNetwork {
    public:
        typedef std::shared_ptr<WireNetwork> Ptr;
        static Ptr create(const std::string& wire_file);
        static Ptr create_raw(const MatrixFr& vertices, const MatrixIr& edges);

    public:
        WireNetwork();
        WireNetwork(const std::string& wire_file);
        WireNetwork(const MatrixFr& vertices, const MatrixIr& edges);

    public:
        size_t get_dim() const { return m_dim; }
        size_t get_num_vertices() const { return m_vertices.rows(); }
        size_t get_num_edges() const { return m_edges.rows(); }

        const MatrixFr& get_vertices() const { return m_vertices; }
        const MatrixIr& get_edges() const { return m_edges; }

        void set_vertices(const MatrixFr& vertices) {
            m_vertices = vertices;
            m_dim = m_vertices.cols();
        }

        void set_edges(const MatrixIr& edges) {
            m_edges = edges;
            m_connectivity.reset();
        }

        void scale(const VectorF& factors);
        void scale_fit(const VectorF& bbox_min, const VectorF& bbox_max);
        void translate(const VectorF& offset);
        void center_at_origin();
        VectorF center() const;
        VectorF get_bbox_min() const { return m_vertices.colwise().minCoeff(); }
        VectorF get_bbox_max() const { return m_vertices.colwise().maxCoeff(); }

        void filter_vertices(const std::vector<bool>& to_keep);
        void filter_edges(const std::vector<bool>& to_keep);


    public:
        bool has_attribute(const std::string& name) const {
            return m_attributes.has_attribute(name);
        }

        bool is_vertex_attribute(const std::string& name) const {
            return m_attributes.is_vertex_attribute(name);
        }

        const MatrixFr& get_attribute(const std::string& name) const {
            return m_attributes.get_attribute(name);
        }

        void add_attribute(const std::string& name, bool vertex_wise=true) const {
            m_attributes.add_attribute(*this, name, vertex_wise);
        }

        void set_attribute(const std::string& name, const MatrixFr& values) const {
            m_attributes.set_attribute(*this, name, values);
        }

        std::vector<std::string> get_attribute_names() const {
            return m_attributes.get_attribute_names();
        }

        void clear_attributes() {
            m_attributes.clear();
        }

    public:
        bool with_connectivity() const {
            return m_connectivity.is_available();
        }

        void compute_connectivity() {
            m_connectivity.compute();
        }

        VectorI get_vertex_neighbors(size_t i) const {
            return m_connectivity.get_vertex_neighbors(i);
        }

    private:
        void initialize();
        void initialize_connectivity();

    private:
        size_t m_dim;
        MatrixFr m_vertices;
        MatrixIr m_edges;
        mutable WireAttributes m_attributes;
        WireConnectivity m_connectivity;
};